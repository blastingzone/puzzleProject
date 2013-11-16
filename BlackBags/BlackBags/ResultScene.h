#pragma once
#include "scene.h"
class CResultScene :
	public CScene
{
public:
	CResultScene(void);
	~CResultScene(void);

	SceneName Update(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
};