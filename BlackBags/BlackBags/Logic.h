#pragma once
#include "GameMap.h"
#include "Player.h"
#include <time.h>

enum Direction{ //SM9: 중괄호 시작 위치 수정.
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
	bool DrawLine(IndexedPosition indexedPosition); //SM9: 넘기는 객체가 함수 안에서 변화가 없을 때는, bool DrawLine(const IndexedPosition& indexedPos) 처럼 넘기는 습관을 가질 것

	IndexedPosition CalcualteIndex(Coordinate mouseCoordinate); 

	bool IsClosed(IndexedPosition indexedPosition, OUT IndexedPosition* tempArray); //SM9: tempArray 가 출력용이라면 OUT 붙일 것, 뭔가 임시 배열을 밖으로 보낸다는것은 별로 좋은 구조는 아님.

	bool IsEnd();

	//맵사이즈는 사실상 좌표를 클릭하는 것이 될텐데 어떻게 받아서 넘겨줄 것인가?
	void GetMapSize();

private:
	static CLogic*	m_pInstance; //singleton instance

	//주어진 index의 울타리 주변 타일을 확인 합니다.
	//SM9: 왜 멤버 함수에 CLogic:: 붙여놨나?
	bool CLogic::ExploreTile(IndexedPosition indexedPosition, IndexedPosition* candidateTIleList, Direction direction);

	//주어진 index의 울타리 주변 타일을 확인 합니다. (switch문이 너무 늘어나고 포인터로 넘길 데이터가 많아서 그냥 구현 안 함)
	//void CLogic::CheckTile(IndexedPosition indexedPosition, IndexedPosition* candidateTIleList, Direction lineDirection, std::queue<IndexedPosition>* searchTiles);

	// IsClosed함수에서 탐색 중인 타일이 이미 탐색된 곳인지 체크하는 함수입니다
	bool IsAlreadyChecked(IndexedPosition* candidateTileList, IndexedPosition nextTile);

	int			m_PlayerNumber;
	CPlayer*	m_Player[4]; //SM9: 배열 크기 지정 상수는 반드시 밖으로 뺄 것. config.h 같은데 몰아 넣기를 추천
	CGameMap*	m_Map;
	int			m_PlayerTurn;
};

