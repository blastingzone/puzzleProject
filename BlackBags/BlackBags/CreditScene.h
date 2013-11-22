#pragma once
#include "scene.h"
class CCreditScene :
	public CScene
{
public:
	CCreditScene(void);
	~CCreditScene(void);

	SceneName Update(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
	void Render();
};

