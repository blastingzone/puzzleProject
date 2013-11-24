#pragma once
#include "scene.h"

struct IDWriteFactory;
struct IDWriteTextFormat;

class CGameResult;

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
	CGameResult* m_GameResult;
};
