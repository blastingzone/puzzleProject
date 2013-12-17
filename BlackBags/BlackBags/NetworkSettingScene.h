#pragma once
#include "scene.h"
#include "NetworkSettingMenu.h"

class CNetworkSettingScene :
	public CScene
{
public:
	CNetworkSettingScene(void);
	virtual ~CNetworkSettingScene(void);

	virtual bool Init();

	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void Render();

	virtual void PlayBGM();
	virtual void StopBGM();

	bool CheckGameStartCondition();

	void GoNextScene();

private:
	CNetworkSettingMenu*	m_SettingMenu;
	int						m_SelectedPlayerNumber;
	int						m_SelectedMapIndex;
};

