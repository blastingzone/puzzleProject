#pragma once
#include "GameMap.h"
#include "Player.h"

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
	
	// IsClosed함수에서 탐색 중인 타일이 이미 탐색된 곳인지 체크하는 함수입니다
	bool IsAlreadyChecked(IndexedPosition* candidateTileList, IndexedPosition nextTile);

	int			m_PlayerNumber;
	CPlayer*	m_Player[4];
	CGameMap*	m_Map;
};

