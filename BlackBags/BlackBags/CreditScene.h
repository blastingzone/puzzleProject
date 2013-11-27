#pragma once
#include "scene.h"
class CCreditScene :
	public CScene
{
public:
	CCreditScene(void);
	~CCreditScene(void);

	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void Render();
	virtual bool Init();

};

