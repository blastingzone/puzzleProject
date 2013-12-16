#include "stdafx.h"
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
	if (clientId != NOT_LOGIN_CLIENT)
	{
		client->SetClientId(clientId);			
	}
	else
		client->Disconnect();
	//GiveClientId();

	return client ;
}



void ClientManager::BroadcastPacket(ClientSession* from, PacketHeader* pkt)
{
	///FYI: C++ STL iterator ��Ÿ���� ����
	for (ClientList::const_iterator it=mClientList.begin() ; it!=mClientList.end() ; ++it)
	{
		ClientSession* client = it->second ;
		
		if ( from == client )
			continue ;
		
		client->Send(pkt) ;
	}
}

void ClientManager::OnPeriodWork()
{
	/// ������ ���� ���ǵ� �ֱ������� ���� (1�� ���� ���� ������)
	DWORD currTick = GetTickCount() ;
	if ( currTick - mLastGCTick >= 1000 )
	{
		CollectGarbageSessions() ;
		mLastGCTick = currTick ;
	}

	/// ���ӵ� Ŭ���̾�Ʈ ���Ǻ��� �ֱ������� ����� �ϴ� �� (�ֱ�� �˾Ƽ� ���ϸ� �� - ������ 1�ʷ� ����)
	if ( currTick - mLastClientWorkTick >= 1000 )
	{
		ClientPeriodWork() ;
		mLastClientWorkTick = currTick ;
	}

	/// ó�� �Ϸ�� DB �۾��� ������ Client�� dispatch
	//DispatchDatabaseJobResults() ;
		
}

void ClientManager::CollectGarbageSessions()
{
	std::vector<ClientSession*> disconnectedSessions ;
	
	///FYI: C++ 11 ���ٸ� �̿��� ��Ÿ��
	std::for_each(mClientList.begin(), mClientList.end(),
		[&](ClientList::const_reference it)
		{
			ClientSession* client = it.second ;

			if ( false == client->IsConnected() && false == client->DoingOverlappedOperation() )
				disconnectedSessions.push_back(client) ;
		}
	) ;
	

	///FYI: C��� ��Ÿ���� ����
// 	for (size_t i=0 ; i<disconnectedSessions.size() ; ++i)
// 	{
// 		ClientSession* client = disconnectedSessions[i] ;
// 		mClientList.erase(client->mSocket) ;
// 		delete client ;
// 	}

}

void ClientManager::ClientPeriodWork()
{
	/// FYI: C++ 11 ��Ÿ���� ����
	for (auto& it : mClientList)
	{
		ClientSession* client = it.second ;
		client->OnTick() ;
	}
}

void ClientManager::DispatchDatabaseJobResults()
{
	/// �׿� �ִ� DB �۾� ó�� ������� ������ Ŭ�󿡰� �ѱ��
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
				/// ����� �ش� DB��û�� �ߴ� Ŭ���̾�Ʈ���� ���� ����� �� ����
				auto& it = mClientList.find(dbResult->mSockKey) ;

				if ( it != mClientList.end() && it->second->IsConnected() )
				{
					/// dispatch here....
					it->second->DatabaseJobDone(dbResult) ;
				}
			}
		}
	
	
		/// �Ϸ�� DB �۾� ���ؽ�Ʈ�� ����������
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
		if (mClientIdList[idx] == false)
		{
			mClientIdList[idx] = true;
			return idx;
		}
	}

	return NOT_LOGIN_CLIENT;
}

bool ClientManager::SetCharacterSelectedStatus(int clientId, int characterId) 
{ 
	assert(clientId < MAX_CLIENT_NUM);

	if (mCharacterSelectStatus[clientId] == NOT_SELECTED || characterId == NOT_SELECTED)
	{
		mCharacterSelectStatus[clientId] = characterId; 
		
		return true;
	}

	return false;
}