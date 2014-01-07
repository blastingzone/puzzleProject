﻿#include "stdafx.h"
#include "BBServer.h"
#include "PacketType.h"
#include "ClientSession.h"
#include "ClientManager.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

ClientManager* GClientManager = nullptr ;

ClientSession* ClientManager::CreateClient(SOCKET sock)
{
	ClientSession* client = new ClientSession(sock) ;
	mClientList.insert(ClientList::value_type(sock, client)) ;

	int clientId = GiveClientId();
	if (clientId == NOT_LOGIN_CLIENT || mCurrentScene == SC_PLAY || mCurrentScene == SC_RESULT)
	{
		//이미 게임이 진행 중이거나 풀방이므로 나가주세요
		client->Disconnect();
	}
	else
	{
		client->SetClientId(clientId);
		mBroadcastList[clientId] = client;
	}

	return client ;
}



void ClientManager::BroadcastPacket(ClientSession* from, PacketHeader* pkt)
{
	/*
	///FYI: C++ STL iterator 스타일의 루프
	for (ClientList::const_iterator it=mClientList.begin() ; it!=mClientList.end() ; ++it)
	{
		ClientSession* client = it->second ;
		
		if ( from == client )
			continue ;
		
		client->Send(pkt) ;
	}
	*/

	//기존 루프로는 뭔가 제대로 방송이 안 돼서 따로 관리 ㅠ
	for (int i = 0; i < MAX_CLIENT_NUM ; ++i)
	{
		if (mBroadcastList[i] != nullptr)
		{
			//if ( from == mBroadcastList[i] )
			//	continue ; 

			mBroadcastList[i]->Send(pkt);
		}
	}
}

void ClientManager::OnPeriodWork()
{
	/// 접속이 끊긴 세션들 주기적으로 정리 (1초 정도 마다 해주자)
	DWORD currTick = GetTickCount() ;
	if ( currTick - mLastGCTick >= 1000 )
	{
		CollectGarbageSessions() ;
		mLastGCTick = currTick ;

		//게임 종료 메시지를 받았고, 모든 접속이 종료가 되면 게임에 관련된 정보들을 초기화한다.
		
		if ( (mCurrentScene == SC_RESULT || mCurrentScene == SC_PLAY) && GetConnectionNum() == 0)
		{
			InitGameCondition();
		}
		/*
		if (GetConnectionNum() == 0)
		{
			InitGameCondition();
		}
		*/
	}

	/// 접속된 클라이언트 세션별로 주기적으로 해줘야 하는 일 (주기는 알아서 정하면 됨 - 지금은 1초로 ㅎㅎ)
	if ( currTick - mLastClientWorkTick >= 1000 )
	{
		ClientPeriodWork() ;
		mLastClientWorkTick = currTick ;
	}

	/// 처리 완료된 DB 작업들 각각의 Client로 dispatch
	//DispatchDatabaseJobResults() ;
		
}

void ClientManager::CollectGarbageSessions()
{
	std::vector<ClientSession*> disconnectedSessions ;
	
	///FYI: C++ 11 람다를 이용한 스타일
	std::for_each(mClientList.begin(), mClientList.end(),
		[&](ClientList::const_reference it)
		{
			ClientSession* client = it.second ;

			if ( false == client->IsConnected() && false == client->DoingOverlappedOperation() )
				disconnectedSessions.push_back(client) ;
		}
	) ;
	

	///FYI: C언어 스타일의 루프
// 	for (size_t i=0 ; i<disconnectedSessions.size() ; ++i)
// 	{
// 		ClientSession* client = disconnectedSessions[i] ;
// 		mClientList.erase(client->mSocket) ;
// 		delete client ;
// 	}

}

void ClientManager::ClientPeriodWork()
{
	/// FYI: C++ 11 스타일의 루프
	for (auto& it : mClientList)
	{
		ClientSession* client = it.second ;
		client->OnTick() ;
	}
}

void ClientManager::DispatchDatabaseJobResults()
{
	/// 쌓여 있는 DB 작업 처리 결과들을 각각의 클라에게 넘긴다
	DatabaseJobContext* dbResult = nullptr ;
	while ( GDatabaseJobManager->PopDatabaseJobResult(dbResult) )
	{
		if ( false == dbResult->mSuccess )
		{
			printf("DB JOB FAIL \n") ;
		}
		else
		{
			if ( typeid(*dbResult) == typeid(CreatePlayerDataContext) )
			{
				CreatePlayerDone(dbResult) ;
			}
			else if ( typeid(*dbResult) == typeid(DeletePlayerDataContext) )
			{
				DeletePlayerDone(dbResult) ;
			}
			else
			{
				/// 여기는 해당 DB요청을 했던 클라이언트에서 직접 해줘야 는 경우다
				auto& it = mClientList.find(dbResult->mSockKey) ;

				if ( it != mClientList.end() && it->second->IsConnected() )
				{
					/// dispatch here....
					it->second->DatabaseJobDone(dbResult) ;
				}
			}
		}
	
	
		/// 완료된 DB 작업 컨텍스트는 삭제해주자
		DatabaseJobContext* toBeDelete = dbResult ;
		delete toBeDelete ;
	}
}

void ClientManager::CreatePlayer(int pid, double x, double y, double z, const char* name, const char* comment)
{
	CreatePlayerDataContext* newPlayerJob = new CreatePlayerDataContext() ;
	newPlayerJob->mPlayerId = pid ;
	newPlayerJob->mPosX = x ;
	newPlayerJob->mPosY = y ;
	newPlayerJob->mPosZ = z ;
	strcpy_s(newPlayerJob->mPlayerName, name) ;
	strcpy_s(newPlayerJob->mComment, comment) ;

	GDatabaseJobManager->PushDatabaseJobRequest(newPlayerJob) ;

}

void ClientManager::DeletePlayer(int pid)
{
	DeletePlayerDataContext* delPlayerJob = new DeletePlayerDataContext(pid) ;
	GDatabaseJobManager->PushDatabaseJobRequest(delPlayerJob) ;
}

void ClientManager::CreatePlayerDone(DatabaseJobContext* dbJob)
{
	CreatePlayerDataContext* createJob = dynamic_cast<CreatePlayerDataContext*>(dbJob) ;

	printf("PLAYER[%d] CREATED: %s \n", createJob->mPlayerId, createJob->mPlayerName) ;
}

void ClientManager::DeletePlayerDone(DatabaseJobContext* dbJob)
{
	DeletePlayerDataContext* deleteJob = dynamic_cast<DeletePlayerDataContext*>(dbJob) ;
	
	printf("PLAYER [%d] DELETED\n", deleteJob->mPlayerId) ;

}

int ClientManager::GiveClientId()
{
	for (int idx = 0; idx < MAX_CLIENT_NUM ; ++idx)
	{
		if (!mClientIdStatusList[idx])
		{
			mClientIdStatusList[idx] = true;
			return idx;
		}
	}

	return NOT_LOGIN_CLIENT;
}

int ClientManager::GetConnectionNum()
{
	int connectionNum = 0;

	for (int idx = 0; idx < MAX_CLIENT_NUM ; ++idx)
	{
		if (mClientIdStatusList[idx])
		{
			++connectionNum;
		}
	}

	return connectionNum;
}

bool ClientManager::SetCharacterSelectedStatus(int clientId, int characterId, const std::wstring& playerName) 
{ 
	//로그인 못 한 놈이면 처리 안 한다
	if (clientId >= 0 && clientId < MAX_CLIENT_NUM)
	{
		//캐릭터가 아직 선택 안 된 상태이면 할당해준다
		if (mCharacterSelectStatus[clientId] == NOT_SELECTED || characterId == NOT_SELECTED)
		{
			mCharacterSelectStatus[clientId] = characterId;
			if (characterId != NOT_SELECTED)
			{
				mPlayerNameTable[clientId] = playerName;
			}
			else
			{
				mPlayerNameTable[clientId] = L"";
			}
			return true;
		}
	}

	return false;
}

void ClientManager::LogOut(int clientId) 
{
	mCharacterSelectStatus[clientId] = NOT_SELECTED;
	mClientIdStatusList[clientId] = false; 
	mBroadcastList[clientId] = nullptr;
	mRandomPlayerTurnTable[mCurrentTurn] = NOT_LOGIN_CLIENT;
}

bool ClientManager::IsReady()
{
	int totalPlayerNumber = GetConnectionNum();
	int readyPlayerNumber = 0;
	
	for (int i = 0; i < totalPlayerNumber; ++i)
	{
		readyPlayerNumber += mReadyTable[i];
	}

	if (readyPlayerNumber == totalPlayerNumber)
		return true;
	
	return false;
}

void ClientManager::RandomTurnGenerate()
{
	std::array<int, MAX_CLIENT_NUM> PlayerTurn = {0, 1, 2, 3};
	std::random_shuffle(PlayerTurn.begin(), PlayerTurn.end());

	int idx = 0;

	for (int i = 0; i < MAX_CLIENT_NUM; ++i)
	{
		if ( mClientIdStatusList[PlayerTurn[i]] )
		{
			mRandomPlayerTurnTable[idx++] = PlayerTurn[i];
		}
	}

	mPlayingNumber = GetConnectionNum();
}

void ClientManager::SetNextTurn()
{
	if (mPlayingNumber ==0 || mCurrentScene != SC_PLAY)
		return;

	do
	{
		// 만약 게임 진행 도중에 나간 사람이 있다면 그 차례는 넘긴다.
		mCurrentTurn = ++mCurrentTurn % mPlayingNumber;
	} while ( !mClientIdStatusList[mCurrentTurn] );
}

int	ClientManager::GetCurrentTurn()
{
	return mCurrentTurn;
}

int ClientManager::GetCurrentTurnClientId()
{
	if (mCurrentScene == SC_PLAY)
		return mRandomPlayerTurnTable[mCurrentTurn];

	return -1; //enum으로 교체
}

void ClientManager::InitGameCondition()
{
	memset(mClientIdStatusList, false, sizeof(mClientIdStatusList) );
	memset(mCharacterSelectStatus, NOT_SELECTED, sizeof(mCharacterSelectStatus) );
	memset(mReadyTable, 0, sizeof(mCharacterSelectStatus) );
	
	for (int i = 0; i < MAX_CLIENT_NUM; ++i)
	{
		mRandomPlayerTurnTable[i] = NOT_LOGIN_CLIENT;
		mBroadcastList[i] = nullptr;
		mPlayerNameTable[i] = L"";
	}

	mCurrentTurn = 0;
	mPlayingNumber = 0;
	mCurrentScene = SC_NOSCENE;
}