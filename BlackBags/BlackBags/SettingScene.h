#pragma once
#include "scene.h"
#include "SettingMenu.h"

class CSettingScene :
	public CScene
{
public:
	CSettingScene(void);
	virtual ~CSettingScene(void);
	
	virtual bool Init();

	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void Render();
	
	virtual void PlayBGM();
	virtual void StopBGM();
	
	bool		 CheckGameStartCondition();

private:
	CSettingMenu*		m_SettingMenu;
	int					m_SelectedPlayerNumber;
	int					m_SelectedMapIndex;
};
