#pragma once
#include "GameMap.h"
#include "Player.h"
#include "Scene.h"
#include <time.h>

class CNetworkPlayScene : public CScene
{
public:
	CNetworkPlayScene(void);
	virtual ~CNetworkPlayScene(void);

	virtual bool Init();

	void StartGame();
	void SetPlayerNumber();
	void LinkPlayers();

	//�� �ʱ�ȭ �� �� ���� ���� �°� ��Ÿ���� ������ ���� ��ġ�ϴ� �޼ҵ�
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

	static CNetworkPlayScene*	m_pInstance; //singleton instance

	/*	�־��� index�� ��Ÿ�� �ֺ� Ÿ���� Ȯ�� �մϴ� */
	void CollectClosedTile(IndexedPosition indexedPosition, Direction direction);

	/*	IsClosed�Լ����� Ž�� ���� Ÿ���� �̹� Ž���� ������ üũ�ϴ� �Լ��Դϴ� */
	bool IsAlreadyChecked(const IndexedPosition& nextTile);

	/*	���콺 Ŭ�� ���� ���� */
	void SetClickArea();

	std::array<CPlayer*, MAX_PLAYER_NUM> m_Player;

	int			m_PlayerNumber;
	CGameMap*	m_Map;
	int			m_PlayerTurn;

	float		m_ClickBuffer;
	float		m_ClickLineWeight;
	float		m_ClickTileSize;

	std::array<IndexedPosition, CHECKLIST_LENGTH> m_ClosedTile;
};

