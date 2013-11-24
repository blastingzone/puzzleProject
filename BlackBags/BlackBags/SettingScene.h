#pragma once
#include "scene.h"
#include "SettingMenu.h"

class CSettingScene :
	public CScene
{
public:
	CSettingScene(void);
	~CSettingScene(void);

	void EventHandle(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
	void Render();

private:
	CSettingMenu* m_SettingMenu;
};
