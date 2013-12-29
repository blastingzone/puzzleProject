#pragma once
#include "sceneobject.h"
#include <array>

struct IDWriteFactory;
struct IDWriteTextFormat;

class CGameResult :
	public CSceneObject
{
public:
	CGameResult(void);
	virtual ~CGameResult(void);

	virtual void Render();
	virtual void ResizeClient();
	virtual void SetObjectSize();
	
	void InitMouseOver();
	void SetMouseOver();

	virtual bool Init();

	RECT		GetGetButtonPosition();

private:
	bool CreateResource();

	/*	현재 화면 스케일에 맞춰서 m_TextFormat 갱신  */
	void RefreshTextSize();
	void CalcStartPosition();

	void CalculateScore();
	void DecideWinner();

	std::array<int, MAX_PLAYER_NUM> m_PlayerScore;
	MO_OWNER m_Winner;

	bool m_ButtonMouseOver;

	/*	게임 맵을 그리기위한 화면 기준 점
		게임 맵의 가장 왼쪽 상단 위치 */
	D2D1_SIZE_F	m_StartPosition;

	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_PlayerNameTextFormat;
	IDWriteTextFormat*		m_PlayerScoreTextFormat;
	IDWriteTextFormat*		m_PlayerItemCountTextFormat;

	float m_HorizontalMargin;
	float m_VerticalMargin;
	float m_VoidSpace;

	float m_SceneTitleHeight;
	float m_SceneTitleWidth;

	float m_PlayerPositionH;
	float m_PlayerPositionMargin;

	float m_PlayerFaceHeight;
	float m_PlayerFaceWidth;

	float m_PlayerNameTextSize;
	float m_PlayerNameTextWidth;

	float m_PlayerScoreTextSize;
	float m_playerScoreWidth;

	float m_PlayerItemPosition;
	float m_PlayerItemHeight;
	float m_playerItemWidth;
	float m_playerItemMargin;

	float m_PlayerMultilyHeight;
	float m_playerMultilyWidth;

	float m_PlayerItemCountPosition;
	float m_PlayerItemCountTextSize;
	float m_playerItemCountWidth;

	float m_CrownPosition;
	float m_CrownHeight;
	float m_CrownWidth;

	float m_SceneFinishPositiontH;
	float m_SceneFinishPositiontV;
	float m_SceneFinishHeight;
	float m_SceneFinishWidth;

	ID2D1SolidColorBrush*	m_pPlayerNameTextBrush;
	ID2D1SolidColorBrush*	m_pPlayerScoreTextBrush;
	ID2D1SolidColorBrush*	m_pPlayerItemCountTextBrush;

	ID2D1Bitmap*			m_pTitleImage;
	ID2D1Bitmap*			m_pFinishImage;
	ID2D1Bitmap*			m_pFinishSelectedImage;
	ID2D1Bitmap*			m_pCrownImage;
	ID2D1Bitmap*			m_pMultiplyImage;
	ID2D1Bitmap*			m_pGoldImage;
	ID2D1Bitmap*			m_pTrashImage;
};

