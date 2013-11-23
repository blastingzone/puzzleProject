#pragma once
#include "scene.h"
class CResultScene :
	public CScene
{
public:
	CResultScene(void);
	~CResultScene(void);

	void EventHandle(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
	void Render();
	void TimeOut();

	void CalculateScore();
	void DecideWinner();

private:
	MO_OWNER m_Winner;
	int m_PlayerScore[MAX_PLAYER_NUM];
};
