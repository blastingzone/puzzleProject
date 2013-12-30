#pragma once
#include "SceneObject.h"

#define MAX_MAPSIZE_NUM 4

struct PlayerSelect
{
	PlayerSelect() :	m_IsSelected(0),
						m_IsMouseOver(0),
						m_ButtonWidth(0.0f),
						m_ButtonHeight(0.0f),
						m_ImgCharacterFace(nullptr),
						m_ImgCharacterFaceSelected(nullptr)
						{};

	bool		m_IsSelected;
	bool		m_IsMouseOver;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	ID2D1Bitmap* m_ImgCharacterFace;
	ID2D1Bitmap* m_ImgCharacterFaceSelected;
};

struct MapSelect
{
	MapSelect() :	m_IsSelected(0),
					m_IsMouseOver(0),
					m_ButtonWidth(0.0f),
					m_ButtonHeight(0.0f),
					m_GameDataMapSizeWidth(0),
					m_GameDataMapSizeHeight(0),
					m_DefaultImgButtonText(nullptr),
					m_SelectedImgButtonText(nullptr)
					{};

	bool		m_IsSelected;
	bool		m_IsMouseOver;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	ID2D1Bitmap* m_DefaultImgButtonText;
	ID2D1Bitmap* m_SelectedImgButtonText;

	int			m_GameDataMapSizeWidth;
	int			m_GameDataMapSizeHeight;

};

struct NextButton
{
	NextButton() :	m_IsPossible(false),
					m_ButtonHeight(0),
					m_ButtonWidth(0),
					m_NextImgButton(nullptr)
					{};

	bool		m_IsPossible;

	float		m_ButtonWidth;
	float		m_ButtonHeight;

	ID2D1Bitmap* m_NextImgButton;

};

struct SettingTitle
{
	SettingTitle() : m_LayerWidth(0.f),
					 m_LayerHeight(0.f),
					 m_Title(nullptr)
					 {};

	float m_LayerWidth;
	float m_LayerHeight;

	ID2D1Bitmap* m_Title;
};

// 껍데기
struct IDWriteFactory;
struct IDWriteTextFormat;

class CSettingMenu :
	public CSceneObject
{
public:
	CSettingMenu(void);
	virtual ~CSettingMenu(void);

	virtual bool Init();
	virtual void ResizeClient();
	virtual void Render();

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

private:
	bool CreateResource();

	void CalcStartPosition();
	void RefreshTextSize();
	virtual void SetObjectSize();

	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_PlayerSelectTextFormat;

	// Player와 Map이 공유하는 브러시
	ID2D1SolidColorBrush*	m_pUnselectedTextBrush;
	ID2D1SolidColorBrush*	m_pSelectedTextBrush;

	D2D1_SIZE_F				m_StartPosition;

	// 체크 아이콘 이미지
	ID2D1Bitmap*			m_SelectedImgCheckIcon;

	// Character 선택창의 이름 부분
	float					m_PlayerSelectTextSize;
	float					m_PlayerSelectTextMargin;
	// 캐릭터 초상화 관련
	float					m_PortraitHeight;
	float					m_PortraitWidth;
	// 체크 아이콘 관련
	float					m_CheckIconWidth;
	float					m_CheckIconHeight;

	SettingTitle			m_SettingTitle;
	NextButton				m_NextButton;

	std::array<PlayerSelect, MAX_PLAYER_NUM>	 m_PlayerSelect;
	std::array<MapSelect, MAX_MAPSIZE_NUM>		m_MapSelect;

	int						m_PlayerMask;
};

