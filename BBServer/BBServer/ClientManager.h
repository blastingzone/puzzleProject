#pragma once

#include <map>
#include <WinSock2.h>
#include <array>

class ClientSession ;
struct PacketHeader ;
struct DatabaseJobContext ;

class ClientManager
{
public:
	ClientManager() : mLastGCTick(0), mLastClientWorkTick(0), mCurrentTurn(0)
	{
		memset(mClientIdList, false, sizeof(mClientIdList) );
		memset(mCharacterSelectStatus, NOT_SELECTED, sizeof(mCharacterSelectStatus) );

		// 조심해!
		// std::array는 선언하면 초기화 된다는데 사실입니까?
		for (int i = 0; i < MAX_CLIENT_NUM; ++i)
		{
			mRandomPlayerTurnTable[i] = -1;
			mBroadcastList[i] = nullptr;
		}
	}

	ClientSession* CreateClient(SOCKET sock) ;
	
	void BroadcastPacket(ClientSession* from, PacketHeader* pkt) ;

	void OnPeriodWork() ;

	/// DB에 플레이어 정보를 생성하거나 삭제하는 함수
	void CreatePlayer(int pid, double x, double y, double z, const char* name, const char* comment) ;
	void DeletePlayer(int pid) ;

	int GiveClientId();
	int GetConnectionNum();
	void LogOut(int clientId);

	int GetCharacterSelectedStatusByClientId(int clientId)	{ return mCharacterSelectStatus[clientId]; }
	bool SetCharacterSelectedStatus(int clientId, int characterId);

	/// Ready State가 되었는지 확인하기 위한 메소드들
	bool IsReady();
	void SetReady(int idx)			{ mReadyTable[idx] = 1; }
	void InitReadyTable()			{ memset( mReadyTable, 0, sizeof(mReadyTable) ); }

	/// 랜덤 턴을 생성하고 유지하기 위한 메소드
	void RandomTurnGenerate();
	int  GetNextTurn()				{ return mRandomPlayerTurnTable[mCurrentTurn++ %  GetConnectionNum()]; }

private:
	void CreatePlayerDone(DatabaseJobContext* dbJob) ;
	void DeletePlayerDone(DatabaseJobContext* dbJob) ;

private:
	void CollectGarbageSessions() ;
	void ClientPeriodWork() ;
	void DispatchDatabaseJobResults() ;

private:
	typedef std::map<SOCKET, ClientSession*> ClientList ;
	ClientList	mClientList ;

	DWORD		mLastGCTick ;
	DWORD		mLastClientWorkTick ;

	bool		mClientIdList[MAX_CLIENT_NUM];

	/*	각 클라이언트들이 몇 번 캐릭터를 선택했는지 저장
		인덱스는 클라이언트 아이디 */
	int			mCharacterSelectStatus[MAX_CLIENT_NUM];

	/*  클라이언트들이 Ready를 보내오면 차곡차곡 저장해서 MAX_CLIENT_NUM이 되면 Go 시킴*/
	int			mReadyTable[MAX_CLIENT_NUM];

	/* 랜덤 턴을 저장하고 다음 턴을 클라이언트에게 보내주기 위한 배열	*/
	int								mCurrentTurn;
	std::array<int, MAX_CLIENT_NUM> mRandomPlayerTurnTable;

	std::array<ClientSession*, MAX_CLIENT_NUM> mBroadcastList;
} ;

extern ClientManager* GClientManager ;