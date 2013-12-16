#pragma once
#include "scene.h"

class CMainMenu;

class CMainScene :
	public CScene
{
public:
	CMainScene(void);
	virtual ~CMainScene(void);

	virtual bool Init();

	virtual void Render();
	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void PlayBGM();
	virtual void StopBGM();

	/*	�޴� ��ư�� ���콺 �����͸� �ø��� ���õǴ� �ð��� ǥ�� */
	virtual void MouseOver(Coordinate mouseCoordinate);

private:
	CMainMenu*	m_MainMenu;
};

