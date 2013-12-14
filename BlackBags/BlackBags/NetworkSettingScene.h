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
	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void EventHandle(Coordinate mouseCoordinate);

	virtual void PlayBGM();
	virtual void StopBGM();

	virtual bool CheckGameStartCondition();

	CNetworkSettingMenu* m_SettingMenu;
};

