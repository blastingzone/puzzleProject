#pragma once
#include "GameMap.h"
#include "Player.h"

class CLogic
{
public:
	CLogic(void);
	~CLogic(void);

	void					Init();
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

	bool IsEnd();

	//�ʻ������ ��ǻ� ��ǥ�� Ŭ���ϴ� ���� ���ٵ� ��� �޾Ƽ� �Ѱ��� ���ΰ�?
	void GetMapSize();

private:

	static CLogic*	m_pInstance; //singleton instance
	int m_PlayerNumber;
	CPlayer* m_Player[4];
	CGameMap* m_Map;
};

