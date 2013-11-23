#pragma once
#include "scene.h"

struct IDWriteFactory;
struct IDWriteTextFormat;

class CResultScene :
	public CScene
{
public:
	CResultScene(void);
	~CResultScene(void);

	void Render();
	void EventHandle(Coordinate mouseCoordinate);

	void MouseOver(Coordinate mouseCoordinate);

private:
	void CalculateScore();
	void DecideWinner();

	MO_OWNER m_Winner;

	int m_PlayerScore[MAX_PLAYER_NUM];
};
