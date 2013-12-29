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