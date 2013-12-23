#pragma once
#include "scene.h"
class CCreditScene :
	public CScene
{
public:
	CCreditScene(void);

	virtual~CCreditScene(void);

	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void Render();
	virtual bool Init();
	
	virtual void PlayBGM() {}
	virtual void StopBGM() {}

};

