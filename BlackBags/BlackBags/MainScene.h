#pragma once
#include "scene.h"

class CMainMenu;

class CMainScene :
	public CScene
{
public:
	CMainScene(void);
	~CMainScene(void);

	virtual bool Init();

	virtual void Render();
	virtual void EventHandle(Coordinate mouseCoordinate);

	/*	�޴� ��ư�� ���콺 �����͸� �ø��� ���õǴ� �ð��� ǥ�� */
	virtual void MouseOver(Coordinate mouseCoordinate);

private:
	CMainMenu*	m_MainMenu;
};

