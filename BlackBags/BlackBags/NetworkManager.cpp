#include "stdafx.h"
#include "NetworkManager.h"
#include "PacketType.h"
#include "NetworkGameTimer.h"

#pragma comment(lib,"ws2_32.lib")

#define WM_SOCKET		104

#define BUFSIZE	1024*10

CNetworkManager* CNetworkManager::m_pInstance = nullptr;

CNetworkManager::CNetworkManager(void) : m_SendBuffer(BUFSIZE), m_RecvBuffer(BUFSIZE)
{
	m_Hwnd = NULL;
	m_ClientId = -1;

	m_ServerAddr = "localhost" ;
	m_Port = 9001 ;
	m_Socket = NULL ;

	m_MapIndex = -1;
	InitCharaterList();
}

CNetworkManager::~CNetworkManager(void)
{
}

CNetworkManager* CNetworkManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CNetworkManager();
	}

	return m_pInstance;
}

bool CNetworkManager::Init(HWND hwnd)
{
	m_Hwnd = hwnd;

	WSADATA WsaDat ;

	int nResult = WSAStartup(MAKEWORD(2,2),&WsaDat) ;
	if ( nResult != 0 )
		return false ;

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) ;
	if ( m_Socket == INVALID_SOCKET )
		return false ;
	
	/// NAGLE 끈다
	int opt = 1 ;
	::setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)) ;

	return true ;
}

void CNetworkManager::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}

bool CNetworkManager::Connect()
{
	// Resolve IP address for hostname
	struct hostent* host ;

	if ( (host=gethostbyname(m_ServerAddr) ) == NULL )
		return false ;
	
	// Set up our socket address structure
	SOCKADDR_IN SockAddr ;
	SockAddr.sin_port = htons(m_Port) ;
	SockAddr.sin_family = AF_INET;
	//SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr) ;
	
	//다른 컴퓨터 연결
	SockAddr.sin_addr.S_un.S_addr = inet_addr( "10.73.38.130" );

	if ( SOCKET_ERROR == connect(m_Socket, (LPSOCKADDR)(&SockAddr), sizeof(SockAddr)) )
	{
		if (GetLastError() != WSAEWOULDBLOCK )
			return false ;
	}

	return true ;
}

/// 패킷처리 
void CNetworkManager::ProcessPacket()
{
	while ( true )
	{
		PacketHeader header ;

		if ( false == m_RecvBuffer.Peek((char*)&header, sizeof(PacketHeader)) )
			break ;

		if (header.mSize > m_RecvBuffer.GetCurrentSize() )
			break ;

		switch ( header.mType )
		{
		case PKT_SC_LOGIN:
			{
				LoginResult recvData ;
				if ( m_RecvBuffer.Read((char*)&recvData, header.mSize) )
				{
					// 패킷처리
					if ( recvData.mPlayerId == -1  )
					{
						/// 여기 걸리면 로그인 실패다.
						ExitProcess(-1) ;
					}
					
					m_ClientId = recvData.mPlayerId;				
				}
				else
				{
					assert(false) ;
				}
			}
			break ;

		case PKT_SC_CHARACTER_SELECT:
			{
				CharacterResult recvData;
				if ( m_RecvBuffer.Read((char*)&recvData, header.mSize) )
				{
					SetPlayerNumber(recvData.mConnectionNum);

					InitCharaterList();

					for (int PlayerIdx = 0; PlayerIdx < MAX_PLAYER_NUM ; ++PlayerIdx)
					{
						//차례대로 확인하면서 PlayerIdx에 해당하는 클라이언트가 선택한 캐릭터가 있으면
						//그 캐릭터 배열에 선택한 PlayerIdx를 기록한다.

						//조심해!!!!
						//패킷에서는 인덱스를 클라이언트 아이디(PlayerIdx)로 사용하고 데이터를 캐릭터 넘버로 기록
						//반면에 클라이언트에서는 캐릭터 넘버를 인덱스로 사용하고 데이터로 클라이언트 아이디를 기록한다
						//심하게 이상하다. 기존 코드와 호완성을 생각해서 패킷 구조의 변경이 필요하다
						if (recvData.mCharacterId[PlayerIdx] != -1)
						{
							m_CharacterIdx[recvData.mCharacterId[PlayerIdx] ] = PlayerIdx;
						}
					}
				}
			}
			break;

		case PKT_SC_MAP_SELECT:
			{
				MapResult recvData;
				if (m_RecvBuffer.Read((char*)&recvData, header.mSize))
				{
					m_MapIndex = recvData.mMapIdx;
				}
			}
			break;

		case PKT_SC_GAME_START:
			{
				GameStartResult recvData;
				if (m_RecvBuffer.Read((char*)&recvData, header.mSize))
				{
					//이걸 씬이 처리하게 해야 하나...우선은 여기서 다음 씬 세팅해서 넘겨버립니다.
					CGameData::GetInstance()->SetNetworkNextSceneFlag(recvData.mStart);

					//초기 맵 그리는데 필요한 랜덤 시드도 넣어줌
					CGameData::GetInstance()->SetNetworkRandomSeed(recvData.randomSeed);	
				}
			}
			break;

		case PKT_SC_IDX:
			{
				EventPositionResult recvData ;
				if ( m_RecvBuffer.Read((char*)&recvData, header.mSize) )
				{
					

				}
				else
				{
					assert(false) ;
				}
			}
			break ;

		case PKT_SC_TURN_START:
			{
				TurnStartResult recvData ;
				if ( m_RecvBuffer.Read((char*)&recvData, header.mSize ) )
				{
					CNetworkManager::GetInstance()->SetCurrentTurnId(recvData.mNextTurnId);
					CNetworkGameTimer::GetInstance()->SetTimerStart();
				}
			}
			break;

		default:
			assert(false) ;
		}

	}
}

void CNetworkManager::AskClientId()
{
	

	LoginRequest sendData ;
	sendData.mPlayerId = -1;

	if ( m_SendBuffer.Write((const char*)&sendData, sendData.mSize) )
	{
		PostMessage(m_Hwnd, WM_SOCKET, 0, FD_WRITE) ;
	}


	int nResult = WSAAsyncSelect(m_Socket, m_Hwnd, WM_SOCKET, (FD_CLOSE|FD_READ|FD_WRITE) ) ;
	if (nResult)
	{
		assert(false) ;
	}
}


void CNetworkManager::Read()
{
	char inBuf[4096] = {0, } ;
					
	int recvLen = recv(m_Socket, inBuf, 4096, 0) ;

	if ( !m_RecvBuffer.Write(inBuf, recvLen) )
	{
		/// 버퍼 꽉찼다. 
		assert(false) ;
	}
					
	ProcessPacket() ;
}

void CNetworkManager::Write()
{
	/// 실제로 버퍼에 있는것들 꺼내서 보내기
	int size = m_SendBuffer.GetCurrentSize() ;
	if ( size > 0 )
	{
		char* data = new char[size] ;
		m_SendBuffer.Peek(data) ;

		int sent = send(m_Socket, data, size, 0) ;
						
		/// 다를수 있다
		if ( sent != size )
			OutputDebugStringA("sent != request\n") ;

		m_SendBuffer.Consume(sent) ;

		delete [] data ;
	}
}

void CNetworkManager::CloseSocket()
{
	MessageBox(m_Hwnd, L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION|MB_OK);
	closesocket(m_Socket);

	//Scene change - > to mainScene
}

void CNetworkManager::PostSendMessage()
{
	PostMessage(m_Hwnd, WM_SOCKET, 0, FD_WRITE);
}

void CNetworkManager::InitCharaterList()
{
	for (int i = 0; i < MAX_PLAYER_NUM ; ++i)
	{
		m_CharacterIdx[i] = -1;
	}
}