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

	void CalculateScore();
	void DecideWinner();

	int m_PlayerScore[MAX_PLAYER_NUM];
	MO_OWNER m_Winner;

	bool m_ButtonMouseOver;

	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_TitleTextFormat;
	IDWriteTextFormat*		m_PlayerNameTextFormat;
	IDWriteTextFormat*		m_PlayerScoreTextFormat;
	IDWriteTextFormat*		m_WinnerTextFormat;
	IDWriteTextFormat*		m_ButtonTextFormat;

	std::wstring m_Title;
	std::wstring m_WinnerText;
	std::wstring m_ButtonText;

	float m_HorizontalMargin;
	float m_VerticalMargin;
	float m_VoidSpace;

	float m_SceneTitleHeight;
	float m_SceneTitleWidth;
	float m_SceneTitleTextSize;

	float m_PlayerNameTextSize;
	float m_PlayerScoreTextSize;
	float m_PlayerScorePosition;
	float m_playerScoreWidth;
	float m_PlayerTileSize;
	float m_PlayerBoxHeight;

	float m_WinnerTextSize;
	float m_WinnerTextWidth;

	float m_ButtonWidth;
	float m_ButtonHeight;
	float m_ButtonTextMagin;
	float m_ButtonTextSize;

	ID2D1SolidColorBrush*	m_pTextBrush;
	ID2D1SolidColorBrush*	m_pWinnerTextBrush;
	ID2D1SolidColorBrush*	m_pWinnerBoxBrush;
	ID2D1SolidColorBrush*	m_pButtonBrush;
};

