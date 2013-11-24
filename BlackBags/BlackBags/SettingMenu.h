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
	//config�� �ִ� #define Player Color�� ����Ű�� ��
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


// ������
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

private:
	void CreateResource();

	void CalcStartPosition();
	void RefreshTextSize();
	void SetObjectSize();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_PlayerSelectTextFormat;
	IDWriteTextFormat*		m_MapSelectTextFormat;

	// Player�� Map�� �����ϴ� �귯��
	ID2D1SolidColorBrush*	m_pUnselectedTextBrush;
	ID2D1SolidColorBrush*	m_pSelectedTextBrush;
	ID2D1SolidColorBrush*	m_pButtonBrush;
	// ���� ����ϴ� �귯��
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

