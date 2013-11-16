#pragma once
#include <dwrite.h>
#include <d2d1.h>
#include "sceneobject.h"

#define BUTTON_NUMBER 5

struct Button
{
	Button() : m_MouseOver(false), m_ButtonText(NULL), m_StrLength(0) {}

	bool m_MouseOver;
	const wchar_t* m_ButtonText;
	UINT32 m_StrLength; 
};

class CMainMenu :
	public CSceneObject
{
public:
	CMainMenu(void);
	~CMainMenu(void);

	void Render();
	void ResizeClient();
	void SetObjectSize();

	void InitMouseOver();
	void SetMouseOver(int idx);

	void CalcStartPosition();

	void Init();

	float		GetButtonWidth() {return m_MenuButtonWidth;}
	float		GetButtonHeight() {return m_MenuButtonHeight;}
	D2D1_SIZE_F GetStartPosition(){return m_StartPosition;}

private:
	bool CreateResource();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	/*	���� ���� �׸������� ȭ�� ���� ��
		���� ���� ���� ���� ��� ��ġ */
	D2D1_SIZE_F	m_StartPosition;

	//���߿� ����� �� �� ������ ����
	/*
	std::string	m_TextNewGame;
	std::string	m_TextNetwork;
	std::string	m_TextCredits;
	std::string	m_TextCinematicMovie;
	std::string	m_TextExit;
	*/
	Button m_ButtonList[BUTTON_NUMBER];

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_TextFormat;

	float m_MenuButtonWidth;
	float m_MenuButtonHeight;
	float m_MenuTextMagin;

	ID2D1SolidColorBrush*	m_pUnselectedTextBrush;
	ID2D1SolidColorBrush*	m_pSelectedTextBrush;
	ID2D1SolidColorBrush*	m_pMenuButtonBrush;
	ID2D1SolidColorBrush*	m_pBackgroundBrush;
};

