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

	virtual bool Init();
	virtual void Render();
	virtual void EventHandle(Coordinate mouseCoordinate);

	virtual void MouseOver(Coordinate mouseCoordinate);
	
	virtual void PlayBGM();
	virtual void StopBGM();

private:
	CGameResult* m_GameResult;
};
