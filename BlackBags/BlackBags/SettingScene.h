#pragma once
#include "scene.h"
class CSettingScene :
	public CScene
{
public:
	CSettingScene(void);
	~CSettingScene(void);

	SceneName Update(Coordinate mouseCoordinate);
};
