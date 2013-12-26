#pragma once
#include "sceneobject.h"
#include <array>

struct IDWriteFactory;
struct IDWriteTextFormat;

class CNetworkGameResult :
	public CSceneObject
{
public:
	CNetworkGameResult(void);
	virtual ~CNetworkGameResult(void);

	virtual void Render();
	virtual void ResizeClient();
	virtual void SetObjectSize();
	
	void InitMouseOver();
	void SetMouseOver();

	virtual bool Init();

	RECT		GetGetButtonPosition();

private:
	bool CreateResource();

	/*	���� ȭ�� �����Ͽ� ���缭 m_TextFormat ����  */
	void RefreshTextSize();
	void CalcStartPosition();

	void CalculateScore();
	void DecideWinner();

	std::array<int, MAX_PLAYER_NUM> m_PlayerScore;
	MO_OWNER m_Winner;

	bool m_ButtonMouseOver;

	/*	���� ���� �׸������� ȭ�� ���� ��
		���� ���� ���� ���� ��� ��ġ */
	D2D1_SIZE_F	m_StartPosition;

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
	float m_PlayerNameTextWidth;
	float m_PlayerNameTextHeight;
	float m_PlayerScoreTextSize;
	float m_PlayerScorePosition;
	float m_playerScoreWidth;
	float m_PlayerTileSize;
	float m_PlayerTileMargin;
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

	ID2D1SolidColorBrush*	m_pGoldBrush;
	ID2D1SolidColorBrush*	m_pTrashBrush;

	//���߿� �ٸ� ���� �����ϴ� �귯�õ��� ���� �������� ���� �����ϵ��� �� ��
	std::array<ID2D1SolidColorBrush*, MAX_PLAYER_NUM> m_TileBrush;
};