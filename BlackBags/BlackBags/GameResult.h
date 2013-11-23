#pragma once
#include "sceneobject.h"

struct IDWriteFactory;
struct IDWriteTextFormat;

class CGameResult :
	public CSceneObject
{
public:
	CGameResult(void);
	~CGameResult(void);

	void Render();
	void ResizeClient();
	void SetObjectSize();
	void SetMouseOver();

	void CalcStartPosition();

	void Init();

	float		GetButtonWidth() {return m_ButtonWidth;}
	float		GetButtonHeight() {return m_ButtonHeight;}

private:
	bool CreateResource();

	/*	현재 화면 스케일에 맞춰서 m_TextFormat 갱신  */
	void RefreshTextSize();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_TitleTextFormat;
	IDWriteTextFormat*		m_PlayerNameTextFormat;
	IDWriteTextFormat*		m_PlayerPointTextFormat;
	IDWriteTextFormat*		m_WinnerTextFormat;
	IDWriteTextFormat*		m_ButtonTextFormat;

	std::wstring m_Title;
	std::wstring m_Winner;
	std::wstring m_Button;

	//나중에 gameData에서 가져 오는 걸로 수정
	std::wstring m_Player1;
	std::wstring m_Player2;
	std::wstring m_Player3;
	std::wstring m_Player4;
	std::wstring m_Player1Point;
	std::wstring m_Player2Point;
	std::wstring m_Player3Point;
	std::wstring m_Player4Point;

	float m_HorizontalMargin;
	float m_VerticalMargin;
	float m_VoidSpace;

	float m_SceneTitleHeight;
	float m_SceneTitleWidth;
	float m_SceneTitleTextSize;

	float m_PlayerNameTextSize;
	float m_PlayerPointTextSize;
	float m_PlayerTileSize;
	float m_PlayerBoxHeight;

	float m_WinnerTextSize;

	float m_ButtonWidth;
	float m_ButtonHeight;
	float m_ButtonTextMagin;
	float m_ButtonTextSize;

	ID2D1SolidColorBrush*	m_pTextBrush;
	ID2D1SolidColorBrush*	m_pWinnerTextBrush;
	ID2D1SolidColorBrush*	m_pWinnerBoxBrush;
	ID2D1SolidColorBrush*	m_pButtonBrush;
};

