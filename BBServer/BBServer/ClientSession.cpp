﻿#include "stdafx.h"
#include "ClientSession.h"
#include "PacketType.h"
#include "ClientManager.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"
#include <time.h>

bool ClientSession::OnConnect(SOCKADDR_IN* addr)
{
	memcpy(&mClientAddr, addr, sizeof(SOCKADDR_IN)) ;

	//////////////////////////////////////////////////////////////////////////
	/// 소켓을 넌블러킹으로 바꾸고
	//////////////////////////////////////////////////////////////////////////
	u_long arg = 1 ;
	// 조심해?
	// 이 앞에 :: 이건 뭘까요 지워도 에러 안나던데 -> 전역 함수 앞에 붙여주는 것입니다
	::ioctlsocket(mSocket, FIONBIO, &arg) ;

	//////////////////////////////////////////////////////////////////////////
	/// nagle 알고리즘 끄기
	//////////////////////////////////////////////////////////////////////////
	int opt = 1 ;
	::setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)) ;

	printf("[DEBUG] Client Connected: IP=%s, PORT=%d\n", inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port)) ;
	
	mConnected = true ;
	//mClientId = -1;

	return PostRecv() ;
}

bool ClientSession::PostRecv()
{
	if ( !IsConnected() )
		return false ;

	DWORD recvbytes = 0 ;
	DWORD flags = 0 ;
	WSABUF buf ;
	buf.len = (ULONG)mRecvBuffer.GetFreeSpaceSize() ;
	buf.buf = (char*)mRecvBuffer.GetBuffer() ;

	memset(&mOverlappedRecv, 0, sizeof(OverlappedIO)) ;
	mOverlappedRecv.mObject = this ;

	/// 비동기 입출력 시작
	if ( SOCKET_ERROR == WSARecv(mSocket, &buf, 1, &recvbytes, &flags, &mOverlappedRecv, RecvCompletion) )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
			return false ;
	}

	IncOverlappedRequest() ;

	return true ;
}

void ClientSession::Disconnect()
{
	if ( !IsConnected() )
		return ;

	printf("[DEBUG] Client Disconnected: IP=%s, PORT=%d\n", inet_ntoa(mClientAddr.sin_addr), ntohs(mClientAddr.sin_port)) ;

	if (mClientId >= 0 && mClientId < MAX_CLIENT_NUM)
	{
		GClientManager->LogOut(mClientId);
	}

	::shutdown(mSocket, SD_BOTH) ;
	::closesocket(mSocket) ;

	mConnected = false ;
}


void ClientSession::OnRead(size_t len)
{
	mRecvBuffer.Commit(len) ;

	/// 패킷 파싱하고 처리
	while ( true )
	{
		/// 패킷 헤더 크기 만큼 읽어와보기
		PacketHeader header ;
		if ( false == mRecvBuffer.Peek((char*)&header, sizeof(PacketHeader)) )
			return ;

		/// 패킷 완성이 되는가? 
		if ( mRecvBuffer.GetStoredSize() < (header.mSize - sizeof(PacketHeader)) )
			return ;

		/// 패킷 핸들링
		switch ( header.mType )
		{
		case PKT_CS_LOGIN:
			{
				LoginRequest inPacket ;
				mRecvBuffer.Read((char*)&inPacket, header.mSize) ;

				//클라이언트 아이디를 나눠준다.
				LoginResult outPacket;
				outPacket.mPlayerId = mClientId;
				Send(&outPacket);

				/// 로그인은 DB 작업을 거쳐야 하기 때문에 DB 작업 요청한다.
				
				LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext(mSocket, inPacket.mPlayerId) ;
				GDatabaseJobManager->PushDatabaseJobRequest(newDbJob) ;	
			}
			break ;

		case PKT_CS_CHARACTER_SELECT:
			{
				CharacterRequest inPacket;
				CharacterResult outPacket;

				mRecvBuffer.Read( (char*)&inPacket, header.mSize );
				// 임계영역 문제 생김!
				if (GClientManager->SetCharacterSelectedStatus(inPacket.mPlayerId, inPacket.mCharacterId) )
				{
					outPacket.mConnectionNum = GClientManager->GetConnectionNum();

					for (int i = 0 ; i < MAX_CLIENT_NUM ; ++i)
					{
						outPacket.mCharacterId[i] = GClientManager->GetCharacterSelectedStatusByClientId(i);
					}

					if (!Broadcast(&outPacket))
						return ;
				}
			}
			break;

		case PKT_CS_MAP_SELECT:
			{
				MapRequest inPacket;
				MapResult	outPacket;

				mRecvBuffer.Read( (char*)&inPacket, header.mSize );
				
				outPacket.mMapIdx = inPacket.mMapIdx;

				if (!Broadcast(&outPacket))
					return;
			}
			break;

		case PKT_CS_GAME_START:
			{
				GameStartRequest	inPacket;
				GameStartResult		outPacket;

				mRecvBuffer.Read( (char*)&inPacket, header.mSize );
				
				outPacket.mStart = inPacket.mStart;
				outPacket.randomSeed = static_cast<unsigned int> (time(NULL) );

				if (!Broadcast(&outPacket))
					return;
			}
			break;

		//아직 작업 전
		case PKT_CS_IDX:
			{
				EventPositionRequest inPacket;
				mRecvBuffer.Read( (char*)&inPacket, header.mSize ) ;

				EventPositionResult outPacket;
				outPacket.m_Xpos = inPacket.m_Xpos;
				outPacket.m_Ypos = inPacket.m_Ypos;

				if ( !Broadcast(&outPacket) )
					return ;
			}
			break;

		default:
			{
				/// 여기 들어오면 이상한 패킷 보낸거다.
				Disconnect() ;
				return ;
			}
			break ;
		}
	}
}

bool ClientSession::Send(PacketHeader* pkt)
{
	if ( !IsConnected() )
		return false ;

	/// 버퍼 용량 부족인 경우는 끊어버림
	if ( false == mSendBuffer.Write((char*)pkt, pkt->mSize) )
	{
		Disconnect() ;
		return false ;
	}

	/// 보낼 데이터가 있는지 검사
	if ( mSendBuffer.GetContiguiousBytes() == 0 )
	{
		/// 방금전에 write 했는데, 데이터가 없다면 뭔가 잘못된 것
		//////////////////////////////////////////////////////////////////////////
		// Disconnect() 하기 전에 assert 호출해도 되는 것인가??
		assert(false) ;
		Disconnect() ;
		return false ;
	}
		
	DWORD sendbytes = 0 ;
	DWORD flags = 0 ;

	WSABUF buf ;
	buf.len = (ULONG)mSendBuffer.GetContiguiousBytes() ;
	buf.buf = (char*)mSendBuffer.GetBufferStart() ;
	
	memset(&mOverlappedSend, 0, sizeof(OverlappedIO)) ;
	mOverlappedSend.mObject = this ;

	// 비동기 입출력 시작
	if ( SOCKET_ERROR == WSASend(mSocket, &buf, 1, &sendbytes, flags, &mOverlappedSend, SendCompletion) )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
			return false ;
	}

	IncOverlappedRequest() ;
	return true ;
}

void ClientSession::OnWriteComplete(size_t len)
{
	/// 보내기 완료한 데이터는 버퍼에서 제거
	mSendBuffer.Remove(len) ;

	/// 얼래? 덜 보낸 경우도 있나? (커널의 send queue가 꽉찼거나, Send Completion이전에 또 send 한 경우?)
	if ( mSendBuffer.GetContiguiousBytes() > 0 )
	{
		assert(false) ;
	}

}

bool ClientSession::Broadcast(PacketHeader* pkt)
{
	if ( !Send(pkt) )
		return false ;

	if ( !IsConnected() )
		return false ;

	GClientManager->BroadcastPacket(this, pkt) ;

	return true ;
}

void ClientSession::OnTick()
{
	/// 클라별로 주기적으로 해야될 일은 여기에

	/// 특정 주기로 DB에 위치 저장
	/// blackbags 에서는 없어도 되는 코드
// 	if ( ++mDbUpdateCount == PLAYER_DB_UPDATE_CYCLE )
// 	{
// 		mDbUpdateCount = 0 ;
// 		UpdatePlayerDataContext* updatePlayer = new UpdatePlayerDataContext(mSocket, mPlayerId) ;
// 
// 		updatePlayer->mPosX = mPosX ;
// 		updatePlayer->mPosY = mPosY ;
// 		updatePlayer->mPosZ = mPosZ ;
// 		strcpy_s(updatePlayer->mComment, "updated_test") ; ///< 일단은 테스트를 위해
// 		GDatabaseJobManager->PushDatabaseJobRequest(updatePlayer) ;
// 	}
	
}

void ClientSession::DatabaseJobDone(DatabaseJobContext* result)
{
	CRASH_ASSERT( mSocket == result->mSockKey ) ;
	

	const type_info& typeInfo = typeid(*result) ;

	if ( typeInfo == typeid(LoadPlayerDataContext) )
	{
		LoadPlayerDataContext* login = dynamic_cast<LoadPlayerDataContext*>(result) ;

		LoginDone(login->mPlayerId, login->mPosX, login->mPosY, login->mPosZ, login->mPlayerName) ;
	
	}
	else if ( typeInfo == typeid(UpdatePlayerDataContext) )
	{
		UpdateDone() ;
	}
	else
	{
		CRASH_ASSERT(false) ;
	}

}

void ClientSession::UpdateDone()
{
	/// 콘텐츠를 넣기 전까지는 딱히 해줄 것이 없다. 단지 테스트를 위해서..
	printf("DEBUG: Player[%d] Update Done\n", mPlayerId) ;
}



void ClientSession::LoginDone(int pid, double x, double y, double z, const char* name)
{
	LoginResult outPacket ;

	outPacket.mPlayerId = mPlayerId = pid ;
	strcpy_s(mPlayerName, name) ;
	strcpy_s(outPacket.mName, name) ;

	Send(&outPacket) ;

	mLogon = true ;
}


///////////////////////////////////////////////////////////

void CALLBACK RecvCompletion(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	ClientSession* fromClient = static_cast<OverlappedIO*>(lpOverlapped)->mObject ;
	
	fromClient->DecOverlappedRequest() ;

	if ( !fromClient->IsConnected() )
		return ;

	/// 에러 발생시 해당 세션 종료
	if ( dwError || cbTransferred == 0 )
	{
		fromClient->Disconnect() ;
		return ;
	}

	/// 받은 데이터 처리
	fromClient->OnRead(cbTransferred) ;

	/// 다시 받기
	if ( false == fromClient->PostRecv() )
	{
		fromClient->Disconnect() ;
		return ;
	}
}


void CALLBACK SendCompletion(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	ClientSession* fromClient = static_cast<OverlappedIO*>(lpOverlapped)->mObject ;

	fromClient->DecOverlappedRequest() ;

	if ( !fromClient->IsConnected() )
		return ;

	/// 에러 발생시 해당 세션 종료
	if ( dwError || cbTransferred == 0 )
	{
		fromClient->Disconnect() ;
		return ;
	}

	fromClient->OnWriteComplete(cbTransferred) ;

}

