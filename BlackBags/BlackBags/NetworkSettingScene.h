#pragma once
#include "settingscene.h"
#include "NetworkSettingMenu.h"

class CNetworkSettingScene :
	public CSettingScene
{
public:
	CNetworkSettingScene(void);
	virtual ~CNetworkSettingScene(void);

	virtual bool Init();
	virtual void EventHandle(Coordinate mouseCoordinate);

	CNetworkSettingMenu* m_SettingMenu;
};

