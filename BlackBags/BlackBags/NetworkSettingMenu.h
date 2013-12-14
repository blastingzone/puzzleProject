#pragma once
#include "SceneObject.h"

#define MAX_MAPSIZE_NUM 4

struct NetworkPlayerSelect
{
	NetworkPlayerSelect() : m_IsSelected(0),
						m_IsMouseOver(0),
						m_ButtonWidth(0.0f),
						m_ButtonHeight(0.0f),
						m_ButtonText(L""),
						m_pBackgroundBrush(nullptr),
						m_pSelectedBackgroundBrush(nullptr),
						m_IsMine(false) 
						{};

	bool		m_IsSelected;
	bool		m_IsMouseOver;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;

	ID2D1SolidColorBrush*	m_pBackgroundBrush;
	ID2D1SolidColorBrush*	m_pSelectedBackgroundBrush;

	bool m_IsMine;
};

struct NetworkMapSelect
{
	NetworkMapSelect() :	m_IsSelected(0),
					m_IsMouseOver(0),
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

struct NetworkNextButton
{
	NetworkNextButton() :	m_IsPossible(false),
					m_ButtonHeight(0),
					m_ButtonWidth(0),
					m_ButtonText(L"Game Start")
					{};

	bool		m_IsPossible;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	std::wstring m_ButtonText;
};

struct NetworkSettingTitle
{
	NetworkSettingTitle() : m_LayerWidth(0.f),
					 m_LayerHeight(0.f),
					 m_Title(L"Setting")
					 {};

	float m_LayerWidth;
	float m_LayerHeight;

	std::wstring m_Title;
};

struct NetworkPlayerTitle
{
	NetworkPlayerTitle() : m_LayerWidth(0.f),
					 m_LayerHeight(0.f),
					 m_Title(L"Character")
					 {};

	float m_LayerWidth;
	float m_LayerHeight;

	std::wstring m_Title;
};

struct NetworkMapTitle
{
	NetworkMapTitle() : m_LayerWidth(0.f),
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

class CNetworkSettingMenu :
	public CSceneObject
{
public:
	CNetworkSettingMenu(void);
	virtual ~CNetworkSettingMenu(void);

	bool			Init();
	virtual void	ResizeClient();
	virtual void	Render();

	D2D1_SIZE_F GetStartPosition()					{ return m_StartPosition; }
	D2D1_SIZE_F GetPlayerSelectButtonSize();
	D2D1_SIZE_F GetMapSelectButtonSize();
	D2D1_SIZE_F GetNextButtonSize();

	void SetPlayerMouseOver(int idx);
	void SetMapMouseOver(int idx);
	void InitMouseOver();

	void SetPlayerSelected(int idx);
	bool GetPlayerSelected(int idx)					{ return m_PlayerSelect[idx].m_IsSelected; }
	void CancelPlayerSelected(int idx);
	
	void SetMapSelected(int idx)					{ m_MapSelect[idx].m_IsSelected = true; }
	bool GetMapSelected(int idx)					{ return m_MapSelect[idx].m_IsSelected; }

	int GetMapSizeHeight(int idx)					{ return m_MapSelect[idx].m_GameDataMapSizeHeight; }
	int GetMapSizeWidth(int idx)					{ return m_MapSelect[idx].m_GameDataMapSizeWidth; }

	void InitMapSelected();

	void SetNextButtonPossible()					{ m_NextButton.m_IsPossible = true; }
	void SetNextButtonImpossible()					{ m_NextButton.m_IsPossible = false; }

	int GetPlayerMask()								{ return m_PlayerMask; }

	bool	GetIsMineFlag(int idx)					{ return m_PlayerSelect[idx].m_IsMine; }
	void	SetIsMineFlag(int idx, bool flag)		{ m_PlayerSelect[idx].m_IsMine = flag; }
	void	InitMineFlag();
	void	PollingCharacterData();

protected:
	bool			CreateResource();
	void			CalcStartPosition();
	void			RefreshTextSize();
	virtual void	SetObjectSize();

	void ErrorHandling();

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

	ID2D1SolidColorBrush*	m_MyCharacterBrush;

	// 캐릭터 얼굴 브러시
	// PlayScene 에서도 쓰는데 우선은 따로 만든다.
	ID2D1Bitmap*			m_pCharacterFace[MAX_PLAYER_NUM];

	D2D1_SIZE_F				m_StartPosition;

	// Character 선택창의 이름 부분
	float					m_PlayerSelectTextSize;
	float					m_PlayerSelectTextMargin;
	// Map 선택창
	float					m_MapSelectTextSize;
	float					m_MapSelectTextMargin;
	// GameStart 버튼
	float					m_NextButtonTextSize;
	float					m_NextButtonTextMargin;
	// PLAYER 타이틀
	float					m_PlayerTitleTextSize;
	float					m_PlayerTitleTextMargin;
	// MAP 타이틀
	float					m_MapTitleTextSize;
	float					m_MapTitleTextMargin;
	// SettingScene 타이틀
	float					m_SettingTitleTextSize;
	float					m_SettingTitleTextMargin;
	// 캐릭터 초상화 관련
	float					m_PortraitHeight;
	float					m_PortraitWidth;
	
	NetworkPlayerTitle		m_PlayerTitle;
	NetworkMapTitle			m_MapTitle;
	NetworkSettingTitle		m_SettingTitle;

	NetworkPlayerSelect		m_PlayerSelect[MAX_PLAYER_NUM];
	NetworkMapSelect		m_MapSelect[MAX_MAPSIZE_NUM];
	NetworkNextButton		m_NextButton;
	
	int						m_PlayerMask;
};

