#pragma once
#include "Player.h"
#include "Scene.h"
#include <time.h>
#include "NetworkGameMap.h"

class CNetworkPlayScene : public CScene
{
public:
	CNetworkPlayScene(void);
	virtual ~CNetworkPlayScene(void);

	virtual bool Init();

	void StartGame();
	void SetPlayerNumber();
	void LinkPlayers();

	//맵 초기화 할 때 유저 수에 맞게 울타리랑 아이템 랜덤 배치하는 메소드
	void InitRandomMap();

	void ShowResult();
	void CheckScore();

	virtual void		EventHandle(Coordinate mouseCoordinate);
	virtual void		EventHandle(IndexedPosition indexedPosition);
	virtual void		MouseOver(Coordinate mouseCoordinate);
	virtual void		Render();
	virtual void		ResizeClient();

	virtual void		PlayBGM();
	virtual void		StopBGM();

	IndexedPosition CalculateIndex(Coordinate mouseCoordinate);

	bool IsPossible(IndexedPosition indexedPosition);
	bool IsClosed(IndexedPosition indexedPosition);

	bool IsEnd();

	void GetMapSize();

	void WriteResult();

	void DrawLineFromServer(const IndexedPosition& indexedPosition, int clientId);

private:
	void TimeOut();

	static CNetworkPlayScene*	m_pInstance; //singleton instance

	/*	주어진 index의 울타리 주변 타일을 확인 합니다 */
	void CollectClosedTile(IndexedPosition indexedPosition, Direction direction);

	/*	IsClosed함수에서 탐색 중인 타일이 이미 탐색된 곳인지 체크하는 함수입니다 */
	bool IsAlreadyChecked(const IndexedPosition& nextTile);

	/*	마우스 클릭 범위 설정 */
	void SetClickArea();

	std::array<CPlayer*, MAX_PLAYER_NUM> m_Player;

	int					m_PlayerNumber;
	CNetworkGameMap*	m_Map;
	int					m_PlayerTurn;

	float				m_ClickBuffer;
	float				m_ClickLineWeight;
	float				m_ClickTileSize;

	std::array<IndexedPosition, CHECKLIST_LENGTH> m_ClosedTile;
};

