#pragma once
#include "GameMap.h"
#include "Player.h"
#include "Scene.h"
#include <time.h>



class CPlayScene : public CScene
{
public:
	CPlayScene(void);
	~CPlayScene(void);

	void Init();

	void StartGame();
	bool GetPlayerNumber();
	bool CreatePlayers();
	bool SetPlayerTurn();

	//맵 초기화 할 때 유저 수에 맞게 울타리랑 아이템 랜덤 배치하는 메소드
	void InitRandomMap();

	void ShowResult();
	void CheckScore();

	SceneName Update(Coordinate mouseCoordinate);
	void Render();

	IndexedPosition CalculateIndex(Coordinate mouseCoordinate);

	bool IsPossible(IndexedPosition indexedPosition);
	bool IsClosed(IndexedPosition indexedPosition);

	bool IsEnd();

	//맵사이즈는 사실상 좌표를 클릭하는 것이 될텐데 어떻게 받아서 넘겨줄 것인가?
	void GetMapSize();

private:
	static CPlayScene*	m_pInstance; //singleton instance

	/*	주어진 index의 울타리 주변 타일을 확인 합니다 */
	bool ExploreTile(IndexedPosition indexedPosition, Direction direction);

	/*	IsClosed함수에서 탐색 중인 타일이 이미 탐색된 곳인지 체크하는 함수입니다 */
	bool IsAlreadyChecked(const IndexedPosition& nextTile);

	int			m_PlayerNumber;
	CPlayer*	m_Player[MAX_PLAYER_NUM];
	CGameMap*	m_Map;
	int			m_PlayerTurn;
	IndexedPosition m_ClosedTile[CHECKLIST_LENGTH];
};

