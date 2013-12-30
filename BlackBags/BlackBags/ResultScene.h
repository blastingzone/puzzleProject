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
	virtual ~CResultScene(void);

	virtual bool Init();
	virtual void Render();
	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	
	virtual void ResizeClient();
	
	virtual void PlayBGM();
	virtual void StopBGM();

private:
	void SetButtonPosition();

	RECT m_ButtonPosition;
	CGameResult* m_GameResult;
};
