#pragma once
#include "scene.h"
#include <d2d1.h>

struct PlayerSelect
{
	PlayerSelect() :	m_IsSelected(0),
						m_ButtonWidth(0.0f),
						m_ButtonHeight(0.0f),
						m_ButtonText(L""),
						m_PlayerId(0),
						m_pUnselectedTextBrush(nullptr),
						m_pSelectedTextBrush(nullptr),
						m_pMenuButtonBrush(nullptr),
						m_pBackgroundBrush(nullptr)
						{};

	bool		m_IsSelected;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;

	int			m_PlayerId;

	ID2D1SolidColorBrush*	m_pUnselectedTextBrush;
	ID2D1SolidColorBrush*	m_pSelectedTextBrush;
	ID2D1SolidColorBrush*	m_pMenuButtonBrush;
	ID2D1SolidColorBrush*	m_pBackgroundBrush;

	D2D1::ColorF::Enum		m_PlayerColor;
};

// ²®µ¥±â
struct IDWriteFactory;
struct IDWriteTextFormat;

class CSettingScene :
	public CScene
{
public:
	CSettingScene(void);
	~CSettingScene(void);

	SceneName Update(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
	void Render();
	void CreateResource();

private:
	void RefreshTextSize();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	float m_MenuTextMagin;
	float m_MenuTextSize;
};
