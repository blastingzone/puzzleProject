#pragma once
#include "scene.h"
class CMainScene :
	public CScene
{
public:
	CMainScene(void);
	~CMainScene(void);

	SceneName Update(Coordinate mouseCoordinate);
};

