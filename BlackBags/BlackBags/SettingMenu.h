#pragma once
#include "sceneobject.h"

#define MAX_MAPSIZE_NUM 4

struct PlayerSelect
{
	PlayerSelect() :	m_IsSelected(0),
						m_ButtonWidth(0.0f),
						m_ButtonHeight(0.0f),
						m_ButtonText(L""),
						m_pBackgroundBrush(nullptr),
						m_pSelectedBackgroundBrush(nullptr)
						{};

	bool		m_IsSelected;
	bool		m_IsMouseOver;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;

	ID2D1SolidColorBrush*	m_pBackgroundBrush;
	ID2D1SolidColorBrush*	m_pSelectedBackgroundBrush;
};

struct MapSelect
{
	MapSelect() :	m_IsSelected(0),
					m_ButtonWidth(0.0f),
					m_ButtonHeight(0.0f),
					m_ButtonText(L""),
					m_GameDataMapSizeWidth(0),
					m_GameDataMapSizeHeight(0)
					{};

	bool		m_IsSelected;
	bool		m_IsMouseOver;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;

	int			m_GameDataMapSizeWidth;
	int			m_GameDataMapSizeHeight;

};

struct NextButton
{
	NextButton() :	m_IsPossible(false),
					m_ButtonHeight(0),
					m_ButtonWidth(0),
					m_ButtonText(L"Game Start")
					{};

	bool		m_IsPossible;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;
};

struct SettingTitle
{
	SettingTitle() : m_LayerWidth(0.f),
					 m_LayerHeight(0.f),
					 m_Title(L"Setting")
					 {};

	float m_LayerWidth;
	float m_LayerHeight;

	std::wstring m_Title;
};

struct PlayerTitle
{
	PlayerTitle() : m_LayerWidth(0.f),
					 m_LayerHeight(0.f),
					 m_Title(L"Player")
					 {};

	float m_LayerWidth;
	float m_LayerHeight;

	std::wstring m_Title;
};

struct MapTitle
{
	MapTitle() : m_LayerWidth(0.f),
				 m_LayerHeight(0.f),
				 m_Title(L"Map")
				 {};

	float m_LayerWidth;
	float m_LayerHeight;

	std::wstring m_Title;
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

	virtual bool Init();
	virtual void ResizeClient();
	virtual void Render();

	D2D1_SIZE_F GetStartPosition() {return m_StartPosition;}
	D2D1_SIZE_F GetPlayerSelectButtonSize();
	D2D1_SIZE_F GetMapSelectButtonSize();
	D2D1_SIZE_F GetNextButtonSize();

	void SetPlayerMouseOver(int idx);
	void SetMapMouseOver(int idx);
	void InitMouseOver();

	void SetPlayerSelected(int idx);
	bool GetPlayerSelected(int idx) {return m_PlayerSelect[idx].m_IsSelected;}
	void CancelPlayerSelected(int idx);
	
	void SetMapSelected(int idx) {m_MapSelect[idx].m_IsSelected = true;}
	bool GetMapSelected(int idx) {return m_MapSelect[idx].m_IsSelected;}

	int GetMapSizeHeight(int idx) {return m_MapSelect[idx].m_GameDataMapSizeHeight;}
	int GetMapSizeWidth(int idx) {return m_MapSelect[idx].m_GameDataMapSizeWidth;}

	void InitMapSelected();

	void SetNextButtonPossible() {m_NextButton.m_IsPossible = true;}
	void SetNextButtonImpossible() {m_NextButton.m_IsPossible = false;}

	int GetPlayerMask()		{ return m_PlayerMask; }

private:
	bool CreateResource();

	void CalcStartPosition();
	void RefreshTextSize();
	virtual void SetObjectSize();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_PlayerSelectTextFormat;
	IDWriteTextFormat*		m_MapSelectTextFormat;
	IDWriteTextFormat*		m_NextButtonTextFormat;
	IDWriteTextFormat*		m_SubTitleTextFormat;
	IDWriteTextFormat*		m_MainTitleTextFormat;

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

	float		m_NextButtonTextSize;
	float		m_NextButtonTextMargin;
	
	float		m_PlayerTitleTextSize;
	float		m_PlayerTitleTextMargin;

	float		m_MapTitleTextSize;
	float		m_MapTitleTextMargin;

	float		m_SettingTitleTextSize;
	float		m_SettingTitleTextMargin;

	PlayerTitle  m_PlayerTitle;
	MapTitle	 m_MapTitle;
	SettingTitle m_SettingTitle;

	PlayerSelect m_PlayerSelect[MAX_PLAYER_NUM];
	MapSelect	m_MapSelect[MAX_MAPSIZE_NUM];
	NextButton  m_NextButton;

	int			m_PlayerMask;
};

