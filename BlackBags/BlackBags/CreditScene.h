#pragma once
#include "scene.h"
class CCreditScene :
	public CScene
{
public:
	CCreditScene(void);
	~CCreditScene(void);

	SceneName Update(Coordinate mouseCoordinate);
};

