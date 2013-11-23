#pragma once
#include "scene.h"

class CMainMenu;

class CMainScene :
	public CScene
{
public:
	CMainScene(void);
	~CMainScene(void);

	void Render();
	void EventHandle(Coordinate mouseCoordinate);

	/*	메뉴 버튼에 마우스 포인터를 올리면 선택되는 시각적 표현 */
	void MouseOver(Coordinate mouseCoordinate);

private:
	CMainMenu*	m_MainMenu;
};

