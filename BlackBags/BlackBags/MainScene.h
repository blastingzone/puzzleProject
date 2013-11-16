#pragma once
#include "scene.h"
#include "MainMenu.h"

class CMainScene :
	public CScene
{
public:
	CMainScene(void);
	~CMainScene(void);

	void	Init();
	void	Render();

	SceneName	Update(Coordinate mouseCoordinate);
	void		MouseOver(Coordinate mouseCoordinate);

private:
	CMainMenu*	m_MainMenu;
};

