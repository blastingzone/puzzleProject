﻿#pragma once
#include "sceneobject.h"

#define BUTTON_NUMBER 5

struct Button
{
	Button() : m_MouseOver(false), m_ButtonText(L""), m_LinkedScene(SC_NOSCENE) {}

	bool m_MouseOver;
	std::wstring m_ButtonText;
	SceneName m_LinkedScene;
};

struct IDWriteFactory;
struct IDWriteTextFormat;

class CMainMenu :
	public CSceneObject
{
public:
	CMainMenu(void);
	virtual ~CMainMenu(void);

	virtual void Render();
	virtual void ResizeClient();
	virtual void SetObjectSize();

	void InitMouseOver();
	void SetMouseOver(int idx);

	SceneName GetLinkedSceneName(int idx);

	void CalcStartPosition();

	virtual bool Init();

	float		GetButtonWidth() {return m_MenuButtonWidth;}
	float		GetButtonHeight() {return m_MenuButtonHeight;}
	D2D1_SIZE_F GetStartPosition() {return m_StartPosition;}

private:
	bool CreateResource();

	/*	현재 화면 스케일에 맞춰서 m_TextFormat 갱신  */
	void RefreshTextSize();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	/*	게임 맵을 그리기위한 화면 기준 점
		게임 맵의 가장 왼쪽 상단 위치 */
	D2D1_SIZE_F	m_StartPosition;

	/*	화면에 표시되는 버튼들 */
	Button m_ButtonList[BUTTON_NUMBER];

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_TextFormat;

	float m_MenuButtonWidth;
	float m_MenuButtonHeight;
	float m_MenuTextMagin;
	float m_MenuTextSize;
	float m_BackgroundWidth;
	float m_BackgroundHeight;

	ID2D1SolidColorBrush*	m_pUnselectedTextBrush;
	ID2D1SolidColorBrush*	m_pSelectedTextBrush;
	ID2D1SolidColorBrush*	m_pMenuButtonBrush;
	ID2D1SolidColorBrush*	m_pBackgroundBrush;

	ID2D1Bitmap *m_pBackgroundImage;
};

