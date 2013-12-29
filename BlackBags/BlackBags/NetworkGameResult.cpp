#include "stdafx.h"
#include "NetworkGameResult.h"
#include <string.h>
#include "dwrite.h"
#include "Player.h"

CNetworkGameResult::CNetworkGameResult(void)
{
	m_StartPosition.height = 0;
	m_StartPosition.width = 0;

	for (int i = 0; i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerScore[i] = 0;
	}
	m_Winner = MO_NOBODY;

	m_ButtonMouseOver = false;

	m_pRenderTarget = nullptr;

	m_DWriteFactory = nullptr;
	m_PlayerNameTextFormat = nullptr;
	m_PlayerScoreTextFormat = nullptr;
	m_PlayerItemCountTextFormat = nullptr;

	m_HorizontalMargin = 0;
	m_VerticalMargin = 0;
	m_VoidSpace = 0;

	m_SceneTitleHeight = 0;
	m_SceneTitleWidth = 0;

	m_PlayerPositionH = 0;
	m_PlayerPositionMargin = 0;
	
	m_PlayerFaceHeight = 0;
	m_PlayerFaceWidth = 0;

	m_PlayerNameTextSize = 0;
	m_PlayerNameTextWidth = 0;

	m_PlayerScoreTextSize = 0;
	m_playerScoreWidth = 0;

	m_PlayerItemPosition = 0;
	m_PlayerItemHeight = 0;
	m_playerItemWidth = 0;
	m_playerItemMargin = 0;

	m_PlayerMultilyHeight = 0;
	m_playerMultilyWidth = 0;

	m_PlayerItemCountPosition = 0;
	m_PlayerItemCountTextSize = 0;
	m_playerItemCountWidth = 0;

	m_SceneFinishPositiontH = 0;
	m_SceneFinishPositiontV = 0;
	m_SceneFinishHeight = 0;
	m_SceneFinishWidth = 0;

	m_CrownPosition = 0;
	m_CrownHeight = 0;
	m_CrownWidth = 0;
	
	m_pPlayerNameTextBrush = nullptr;
	m_pPlayerScoreTextBrush = nullptr;
	m_pPlayerItemCountTextBrush = nullptr;

	m_pTitleImage = nullptr;
	m_pFinishImage = nullptr;
	m_pFinishSelectedImage = nullptr;
	m_pCrownImage = nullptr;
	m_pMultiplyImage = nullptr;
	m_pGoldImage = nullptr;
	m_pTrashImage = nullptr;
}


CNetworkGameResult::~CNetworkGameResult(void)
{
	SafeRelease(m_DWriteFactory);

	SafeRelease(m_PlayerNameTextFormat);
	SafeRelease(m_PlayerScoreTextFormat);
	SafeRelease(m_PlayerItemCountTextFormat);

	SafeRelease(m_pPlayerNameTextBrush);
	SafeRelease(m_pPlayerScoreTextBrush);
	SafeRelease(m_pPlayerItemCountTextBrush);

	SafeRelease(m_pTitleImage);
	SafeRelease(m_pFinishImage);
	SafeRelease(m_pFinishSelectedImage);
	SafeRelease(m_pCrownImage);
	SafeRelease(m_pMultiplyImage);
	SafeRelease(m_pGoldImage);
	SafeRelease(m_pTrashImage);
}

void CNetworkGameResult::Render()
{
	D2D1_RECT_F		rectElement, textPosition;
	D2D1_POINT_2F	pos;
	CPlayer* tempPlayer = nullptr;

	//background
	m_pRenderTarget->DrawBitmap(CGameData::GetInstance()->GetBackgroundImage(), D2D1::RectF(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT) );

	//title part
	pos.x =  m_StartPosition.width + m_HorizontalMargin;
	pos.y =  m_StartPosition.height + m_VerticalMargin;

	rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_SceneTitleWidth, pos.y + m_SceneTitleHeight);

	m_pRenderTarget->DrawBitmap(m_pTitleImage, rectElement);

	//player part
	for (int i = 0; i < CGameData::GetInstance()->GetplayerNumber(); ++i)
	{
		tempPlayer = CGameData::GetInstance()->GetNetworkClinent(i);

		if (tempPlayer == nullptr)
		{
			//게임에 오류가 있다 >>> 종료하자!
			ErrorHandling();
		}
		
		//player face
		pos.x =  m_StartPosition.width + m_PlayerPositionH;
		pos.y =  m_StartPosition.height + m_VerticalMargin + m_SceneTitleHeight + m_VoidSpace 
			+ ( (m_PlayerFaceHeight + m_PlayerPositionMargin) * i);
		
		rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_PlayerFaceWidth, pos.y + m_PlayerFaceHeight);

		m_pRenderTarget->DrawBitmap(tempPlayer->GetPlayerResultFace(), rectElement);

		//player name
		textPosition = D2D1::Rect(
			pos.x + m_PlayerFaceWidth, 
			pos.y, 
			pos.x + m_PlayerNameTextWidth, 
			pos.y + m_PlayerNameTextSize
			);

		m_pRenderTarget->DrawText(
			tempPlayer->GetPlayerName().c_str(),
			tempPlayer->GetPlayerName().length(),
			m_PlayerNameTextFormat,
			textPosition,
			m_pPlayerNameTextBrush
			);

		//player score
		std::wstring playerScore = std::to_wstring(m_PlayerScore[i]);

		textPosition = D2D1::Rect(
			pos.x + m_PlayerFaceWidth, 
			pos.y + m_PlayerNameTextSize, 
			pos.x + m_playerScoreWidth, 
			pos.y + m_PlayerFaceHeight
			);
		
		m_pRenderTarget->DrawText(
			playerScore.c_str(),
			playerScore.length(),
			m_PlayerScoreTextFormat,
			textPosition,
			m_pPlayerScoreTextBrush
			);

		//tile part
		std::wstring playerTileCount = std::to_wstring(tempPlayer->GetPlayerItemNumber(MO_NOTHING) );

		//image
		pos.x =  textPosition.right + m_PlayerItemPosition;

		rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_playerItemWidth, pos.y + m_PlayerItemHeight);

		m_pRenderTarget->DrawBitmap(tempPlayer->GetPlayerResultTile(), rectElement);

		//multiply
		pos.x =  rectElement.right;
		
		rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_playerMultilyWidth, pos.y + m_PlayerMultilyHeight);

		m_pRenderTarget->DrawBitmap(m_pMultiplyImage, rectElement);

		//count
		pos.x =  rectElement.right;
		
		textPosition = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_playerItemCountWidth, 
			pos.y + m_PlayerMultilyHeight
			);

		m_pRenderTarget->DrawText(
			playerTileCount.c_str(),
			playerTileCount.length(),
			m_PlayerItemCountTextFormat,
			textPosition,
			m_pPlayerItemCountTextBrush
			);

		//gold part
		std::wstring playerGoldCount = std::to_wstring(tempPlayer->GetPlayerItemNumber(MO_GOLD) );

		//image
		pos.x =  textPosition.right;

		rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_playerItemWidth, pos.y + m_PlayerItemHeight);

		m_pRenderTarget->DrawBitmap(m_pGoldImage, rectElement);

		//multiply
		pos.x =  rectElement.right;
		
		rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_playerMultilyWidth, pos.y + m_PlayerMultilyHeight);

		m_pRenderTarget->DrawBitmap(m_pMultiplyImage, rectElement);

		//count
		pos.x =  rectElement.right;
		
		textPosition = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_playerItemCountWidth, 
			pos.y + m_PlayerMultilyHeight
			);

		m_pRenderTarget->DrawText(
			playerGoldCount.c_str(),
			playerGoldCount.length(),
			m_PlayerItemCountTextFormat,
			textPosition,
			m_pPlayerItemCountTextBrush
			);

		//trash part
		std::wstring playerTrashCount = std::to_wstring(tempPlayer->GetPlayerItemNumber(MO_TRASH) );

		//image
		pos.x =  textPosition.right;

		rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_playerItemWidth, pos.y + m_PlayerItemHeight);

		m_pRenderTarget->DrawBitmap(m_pTrashImage, rectElement);

		//multiply
		pos.x =  rectElement.right;
		
		rectElement = D2D1::Rect(pos.x, pos.y, pos.x + m_playerMultilyWidth, pos.y + m_PlayerMultilyHeight);

		m_pRenderTarget->DrawBitmap(m_pMultiplyImage, rectElement);

		//count
		pos.x =  rectElement.right;
		
		textPosition = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_playerItemCountWidth, 
			pos.y + m_PlayerMultilyHeight
			);

		m_pRenderTarget->DrawText(
			playerTrashCount.c_str(),
			playerTrashCount.length(),
			m_PlayerItemCountTextFormat,
			textPosition,
			m_pPlayerItemCountTextBrush
			);
	}

	//winner's crown
	pos.x = m_StartPosition.width + m_StartPosition.width + m_PlayerPositionH;
	pos.y = m_StartPosition.height + m_VerticalMargin + m_SceneTitleHeight + m_VoidSpace 
			+ ( (m_PlayerMultilyHeight + m_PlayerPositionMargin) * static_cast<int> (m_Winner) )
			- m_CrownPosition;

	rectElement = D2D1::Rect(
		pos.x, 
		pos.y, 
		pos.x + m_CrownWidth, 
		pos.y + m_CrownHeight);

	m_pRenderTarget->DrawBitmap(m_pCrownImage, rectElement);

	//button part
	pos.x = m_StartPosition.width + m_SceneFinishPositiontH;
	pos.y = m_StartPosition.width + m_SceneFinishPositiontV;
	
	rectElement = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_SceneFinishWidth, 
			pos.y + m_SceneFinishHeight
			);

	if (m_ButtonMouseOver)
	{
		m_pRenderTarget->DrawBitmap(m_pFinishSelectedImage, rectElement);
	}
	else
	{
		m_pRenderTarget->DrawBitmap(m_pFinishImage, rectElement);
	}
}

bool CNetworkGameResult::Init()
{
	if (!CreateResource() )
	{
		return false;
	}

	ResizeClient();

	CalculateScore();
	DecideWinner();

	return true;
}

void CNetworkGameResult::ResizeClient()
{
	//화면 크기 조절
	CalcStartPosition();
	SetObjectSize();
	RefreshTextSize();
}

void CNetworkGameResult::CalcStartPosition()
{
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	/*	현재 화면의 왼쪽 상단을 기준점으로 사용 */
	D2D1_SIZE_F centerPosition = m_pRenderTarget->GetSize();
	
	centerPosition.width /= 2;
	centerPosition.height /= 2;

	m_StartPosition.width = centerPosition.width - (WINDOW_WIDTH * tempScale) / 2;
	m_StartPosition.height = centerPosition.height - (WINDOW_HEIGHT * tempScale) / 2;
}

void CNetworkGameResult::SetObjectSize()
{
	/*	현재 렌더러에 저장된 화면 스케일에 맞춰서 
	렌더링 할 때 사용된 오브젝트들 크기 조정 */
	
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_HorizontalMargin = tempScale * SC_RT_HORIZONTAL_MARGIN;
	m_VerticalMargin = tempScale * SC_RT_VERTICAL_MARGIN;
	m_VoidSpace = tempScale * SC_RT_VOID_SPACE;

	m_SceneTitleHeight = tempScale * SC_RT_TITLE_HEIGHT;
	m_SceneTitleWidth = tempScale * SC_RT_TITLE_WIDTH;

	m_PlayerPositionH = tempScale * SC_RT_PLAYER_POSITION_H;
	m_PlayerPositionMargin = tempScale * SC_RT_PLAYER_POSITION_MARGIN;

	m_PlayerFaceHeight = tempScale * SC_RT_PLAYER_FACE_HEIGHT;
	m_PlayerFaceWidth = tempScale * SC_RT_PLAYER_FACE_WIDTH;

	m_PlayerNameTextSize = tempScale * SC_RT_PLAYER_NAME_TEXT_SIZE;
	m_PlayerNameTextWidth = tempScale * SC_RT_PLAYER_NAME_TEXT_WIDTH;

	m_PlayerScoreTextSize = tempScale * SC_RT_PLAYER_SCORE_TEXT_SIZE;
	m_playerScoreWidth = tempScale * SC_RT_PLAYER_SCORE_WIDTH;

	m_PlayerItemPosition = tempScale * SC_RT_PLAYER_ITEM_POSITION;
	m_PlayerItemHeight = tempScale * SC_RT_PLAYER_ITEM_HEIGHT;
	m_playerItemWidth = tempScale * SC_RT_PLAYER_ITEM_WIDTH;
	m_playerItemMargin = tempScale * SC_RT_PLAYER_ITEM_MARGIN;

	m_PlayerMultilyHeight = tempScale * SC_RT_MULTIPLY_HEIGHT;
	m_playerMultilyWidth = tempScale * SC_RT_MULTIPLY_WIDTH;

	m_PlayerItemCountPosition = tempScale * SC_RT_PLAYER_ITEM_COUNT_POSITION;
	m_PlayerItemCountTextSize = tempScale * SC_RT_PLAYER_ITEM_COUNT_SIZE;
	m_playerItemCountWidth = tempScale * SC_RT_PLAYER_ITEM_COUNT_WIDTH;

	m_CrownPosition = tempScale * SC_RT_CROWN_POSITION;
	m_CrownHeight = tempScale * SC_RT_CROWN_HEIGHT;
	m_CrownWidth = tempScale * SC_RT_CROWN_WIDTH;
	
	m_SceneFinishPositiontH = tempScale * SC_RT_FINISH_POSITION_H;
	m_SceneFinishPositiontV = tempScale * SC_RT_FINISH_POSITION_V;
	m_SceneFinishHeight = tempScale * SC_RT_FINISH_HEIGHT;
	m_SceneFinishWidth = tempScale * SC_RT_FINISH_WIDTH;
}

void CNetworkGameResult::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_PlayerNameTextFormat);
	SafeRelease(m_PlayerScoreTextFormat);
	SafeRelease(m_PlayerItemCountTextFormat);

	hr = m_DWriteFactory->CreateTextFormat(
		_UI_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerNameTextSize,
		L"ko",
		&m_PlayerNameTextFormat
		);

	if (!SUCCEEDED(hr) )
		ErrorHandling();
	
	hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	if (!SUCCEEDED(hr) )
		ErrorHandling();

	hr = m_DWriteFactory->CreateTextFormat(
		_UI_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerScoreTextSize,
		L"ko",
		&m_PlayerScoreTextFormat
		);
	
	if (!SUCCEEDED(hr) )
		ErrorHandling();
	
	hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	
	if (!SUCCEEDED(hr) )
		ErrorHandling();

	hr = m_DWriteFactory->CreateTextFormat(
		_UI_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerItemCountTextSize,
		L"ko",
		&m_PlayerItemCountTextFormat
		);
	
	if (!SUCCEEDED(hr) )
		ErrorHandling();
	
	hr = m_PlayerItemCountTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	
	if (!SUCCEEDED(hr) )
		ErrorHandling();
}

bool CNetworkGameResult::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_RESULT_TEXT_) ), &m_pPlayerNameTextBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_RESULT_TEXT_) ), &m_pPlayerScoreTextBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_RESULT_TEXT_) ), &m_pPlayerItemCountTextBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();

		hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_DWriteFactory)
            );
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();

		SetObjectSize();
		RefreshTextSize();

		m_pTitleImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/RESULT_result.png", m_pTitleImage);
		m_pFinishImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/RESULT_finish.png", m_pFinishImage);
		m_pFinishSelectedImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/RESULT_finish_selected.png", m_pFinishSelectedImage);
		m_pCrownImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/RESULT_crown.png", m_pCrownImage);
		m_pMultiplyImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/RESULT_multiply.png", m_pMultiplyImage);
		m_pGoldImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/RESULT_gold.png", m_pGoldImage);
		m_pTrashImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/RESULT_trash.png", m_pTrashImage);
	}

	return true;
}


void CNetworkGameResult::CalculateScore()
{
	CPlayer* tempPlayer = nullptr;

	for (int clientId = 0; clientId < CGameData::GetInstance()->GetplayerNumber(); ++clientId)
	{
		tempPlayer = CGameData::GetInstance()->GetNetworkClinent(clientId);

		if (tempPlayer == nullptr)
		{
			//게임에 오류가 있다 >>> 종료하자!
			CGameData::GetInstance()->SetCurrentScene(SC_EXIT);
			return;
		}

		int	totalScore = tempPlayer->GetPlayerItemNumber(MO_NOTHING) * SC_RT_SCORE_TILE
							+ tempPlayer->GetPlayerItemNumber(MO_GOLD) * SC_RT_SCORE_GOLD
							+ tempPlayer->GetPlayerItemNumber(MO_TRASH) * SC_RT_SCORE_TRASH;
		
		m_PlayerScore[clientId] = totalScore;
	}
}

void CNetworkGameResult::DecideWinner()
{
	int winnerId = 0;

	for (int i = 1; i < CGameData::GetInstance()->GetplayerNumber(); ++i)
	{
		if (m_PlayerScore[i] > m_PlayerScore[winnerId])
		{
			winnerId = i;
		}
	}

	m_Winner = static_cast<MO_OWNER>(winnerId);
}

void CNetworkGameResult::InitMouseOver()
{
	m_ButtonMouseOver = false;
}

void CNetworkGameResult::SetMouseOver()
{
	m_ButtonMouseOver = true;
}

RECT CNetworkGameResult::GetGetButtonPosition()
{
	RECT buttonPosition;

	buttonPosition.left = static_cast<LONG>(CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_HorizontalMargin - m_SceneFinishWidth);
	buttonPosition.right = buttonPosition.left + static_cast<LONG>(m_SceneFinishWidth);
	buttonPosition.top = static_cast<LONG>(CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height - m_VerticalMargin - m_SceneFinishHeight);
	buttonPosition.bottom = buttonPosition.top + static_cast<LONG>(m_SceneFinishHeight);

	return buttonPosition;
}