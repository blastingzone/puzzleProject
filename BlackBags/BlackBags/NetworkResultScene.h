#pragma once
#include "scene.h"

struct IDWriteFactory;
struct IDWriteTextFormat;

class CNetworkGameResult;

class CNetworkResultScene :
	public CScene
{
public:
	CNetworkResultScene(void);
	virtual ~CNetworkResultScene(void);
	
	virtual bool Init();
	virtual void Render();
	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	
	virtual void ResizeClient();
	
	virtual void PlayBGM();
	virtual void StopBGM();

private:
	void SetButtonPosition();

	RECT ButtonPosition;
	CNetworkGameResult* m_GameResult;
};

