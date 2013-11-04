#pragma once
#include "GameMap.h"
#include "Player.h"
#include <time.h>

enum Direction{
	DI_UP,
	DI_RIGHT,
	DI_DOWN,
	DI_LEFT
};

const int CHECKED_TILE_ARRAY_SIZE = 100;

class CLogic
{
public:
	CLogic(void);
	~CLogic(void);

	void				Init();
	static CLogic*		GetInstance();
	static bool			ReleaseInstance();

	bool Release();

	void StartGame();
	bool GetPlayerNumber();
	bool CreatePlayers();
	bool SetPlayerTurn();

	//맵 초기화 할 때 유저 수에 맞게 울타리랑 아이템 랜덤 배치하는 메소드
	void InitRandomMap();

	void ShowResult();
	void CheckScore();

	void Update(Coordinate mouseCoordinate);
	bool DrawLine(IndexedPosition indexedPosition);

	IndexedPosition CalcualteIndex(Coordinate mouseCoordinate);

	bool IsClosed(IndexedPosition indexedPosition, IndexedPosition* tempArray);

	bool IsEnd();

	//맵사이즈는 사실상 좌표를 클릭하는 것이 될텐데 어떻게 받아서 넘겨줄 것인가?
	void GetMapSize();

private:
	static CLogic*	m_pInstance; //singleton instance

	//주어진 index의 울타리 주변 타일을 확인 합니다.
	bool CLogic::ExploreTile(IndexedPosition indexedPosition, IndexedPosition* candidateTIleList, Direction direction);

	//주어진 index의 울타리 주변 타일을 확인 합니다. (switch문이 너무 늘어나고 포인터로 넘길 데이터가 많아서 그냥 구현 안 함)
	//void CLogic::CheckTile(IndexedPosition indexedPosition, IndexedPosition* candidateTIleList, Direction lineDirection, std::queue<IndexedPosition>* searchTiles);

	// IsClosed함수에서 탐색 중인 타일이 이미 탐색된 곳인지 체크하는 함수입니다
	bool IsAlreadyChecked(IndexedPosition* candidateTileList, IndexedPosition nextTile);

	int			m_PlayerNumber;
	CPlayer*	m_Player[4];
	CGameMap*	m_Map;
	int			m_PlayerTurn;
};

