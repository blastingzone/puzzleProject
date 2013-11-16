#pragma once
#include <dwrite.h>
#include <d2d1.h>
#include "sceneobject.h"

class CMainMenu :
	public CSceneObject
{
public:
	CMainMenu(void);
	~CMainMenu(void);

	void Render();
	void ResizeClient();
	void SetObjectSize();

	void CalcStartPosition();

	void Init();

private:
	bool CreateResource();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	/*	게임 맵을 그리기위한 화면 기준 점
		게임 맵의 가장 왼쪽 상단 위치 */
	D2D1_SIZE_F	m_StartPosition;

	//나중에 상수로 뺄 수 있으면 빼자
	std::string	m_TextNewGame;
	std::string	m_TextNetwork;
	std::string	m_TextCredits;
	std::string	m_TextCinematicMovie;
	std::string	m_TextExit;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_TextFormat;

	float m_MenuButtonWidth;
	float m_MenuButtonHeight;

	ID2D1SolidColorBrush*	m_pTextBrush;
	ID2D1SolidColorBrush*	m_pMenuButtonBrush;
	ID2D1SolidColorBrush*	m_pBackgroundBrush;
};

