#pragma once
#include "scene.h"
#include "MainMenu.h"

class CMainScene :
	public CScene
{
public:
	CMainScene(void);
	~CMainScene(void);

	void Render();
	void EventHandle(Coordinate mouseCoordinate);

	/*	�޴� ��ư�� ���콺 �����͸� �ø��� ���õǴ� �ð��� ǥ�� */
	void MouseOver(Coordinate mouseCoordinate);

private:
	CMainMenu*	m_MainMenu;
};

