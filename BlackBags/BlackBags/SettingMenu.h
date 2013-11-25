#pragma once
#include "sceneobject.h"

#define MAX_MAPSIZE_NUM 4

struct PlayerSelect
{
	PlayerSelect() :	m_IsSelected(0),
						m_ButtonWidth(0.0f),
						m_ButtonHeight(0.0f),
						m_ButtonText(L""),
						m_PlayerId(0)
						{};

	bool		m_IsSelected;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;

	int			m_PlayerId;

	ID2D1SolidColorBrush*	m_pBackgroundBrush;
	ID2D1SolidColorBrush*	m_pSelectedBackgroundBrush;
	//config에 있는 #define Player Color를 가리키게 됨
	std::wstring			m_PlayerColor;
};

struct MapSelect
{
	MapSelect() :	m_IsSelected(0),
					m_ButtonWidth(0.0f),
					m_ButtonHeight(0.0f),
					m_ButtonText(L"")
					{};

	bool		m_IsSelected;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;

	int			m_GameDataMapSizeWidth;
	int			m_GameDataMapSizeHeight;

};


// 껍데기
struct IDWriteFactory;
struct IDWriteTextFormat;

class CSettingMenu :
	public CSceneObject
{
public:
	CSettingMenu(void);
	~CSettingMenu(void);

	void ResizeClient();
	void Render();

	D2D1_SIZE_F GetStartPosition() {return m_StartPosition;}
	D2D1_SIZE_F GetPlayerSelectButtonSize();
	D2D1_SIZE_F GetMapSelectButtonSize();

	void SetPlayerMouseOver(int idx);
	void SetMapMouseOver(int idx);
	void InitMouseOver();

private:
	bool CreateResource();

	void CalcStartPosition();
	void RefreshTextSize();
	void SetObjectSize();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_PlayerSelectTextFormat;
	IDWriteTextFormat*		m_MapSelectTextFormat;

	// Player와 Map이 공유하는 브러시
	ID2D1SolidColorBrush*	m_pUnselectedTextBrush;
	ID2D1SolidColorBrush*	m_pSelectedTextBrush;
	ID2D1SolidColorBrush*	m_pButtonBrush;
	// 각자 사용하는 브러시
	ID2D1SolidColorBrush*	m_pMapBackgroundBrush;
	ID2D1SolidColorBrush*	m_pMapSelectedBackgroundBrush;

	D2D1_SIZE_F	m_StartPosition;

	float		m_PlayerSelectTextSize;
	float		m_PlayerSelectTextMargin;

	float		m_MapSelectTextSize;
	float		m_MapSelectTextMargin;

	PlayerSelect m_PlayerSelect[MAX_PLAYER_NUM];
	MapSelect	m_MapSelect[MAX_MAPSIZE_NUM];
};

