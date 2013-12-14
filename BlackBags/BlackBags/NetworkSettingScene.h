#pragma once
#include "settingscene.h"
class CNetworkSettingScene :
	public CSettingScene
{
public:
	CNetworkSettingScene(void);
	virtual ~CNetworkSettingScene(void);

	virtual bool Init();
	virtual void EventHandle(Coordinate mouseCoordinate);

};

