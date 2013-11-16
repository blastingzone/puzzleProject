#pragma once
#include "Scene.h"
class COpeningScene :
	public CScene
{
public:
	COpeningScene(void);
	~COpeningScene(void);

	SceneName Update(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
};
