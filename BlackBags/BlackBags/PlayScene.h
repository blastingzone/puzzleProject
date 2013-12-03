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

	virtual bool Init();

	void StartGame();
	void SetPlayerNumber();
	void CreatePlayers();
	void DeletePlayers();

	//맵 초기화 할 때 유저 수에 맞게 울타리랑 아이템 랜덤 배치하는 메소드
	void InitRandomMap();

	void ShowResult();
	void CheckScore();

	virtual void		EventHandle(Coordinate mouseCoordinate);
	virtual void		EventHandle(IndexedPosition indexedPosition);
	virtual void		MouseOver(Coordinate mouseCoordinate);
	virtual void		Render();
	virtual void		ResizeClient();
	
	virtual void PlayBGM();
	virtual void StopBGM();

	IndexedPosition CalculateIndex(Coordinate mouseCoordinate);

	bool IsPossible(IndexedPosition indexedPosition);
	bool IsClosed(IndexedPosition indexedPosition);

	bool IsEnd();

	void GetMapSize();

	void WriteResult();

private:
	void TimeOut();

	static CPlayScene*	m_pInstance; //singleton instance

	/*	주어진 index의 울타리 주변 타일을 확인 합니다 */
	void CollectClosedTile(IndexedPosition indexedPosition, Direction direction);

	/*	IsClosed함수에서 탐색 중인 타일이 이미 탐색된 곳인지 체크하는 함수입니다 */
	bool IsAlreadyChecked(const IndexedPosition& nextTile);

	/*	마우스 클릭 범위 설정 */
	void SetClickArea();
	
	int			m_PlayerNumber;
	CPlayer*	m_Player[MAX_PLAYER_NUM];
	CGameMap*	m_Map;
	int			m_PlayerTurn;

	float		m_ClickBuffer;
	float		m_ClickLineWeight;
	float		m_ClickTileSize;

	IndexedPosition m_ClosedTile[CHECKLIST_LENGTH]; //sm9: 항상 배열 넘어가는 경우가 안생기도록 방어적으로 코딩할 것. 
	// 제일 좋은 방법은 std::array 같은거 써서 배열 범위 넘어가더라도 바로 알 수 있게 만드는 것.
};

