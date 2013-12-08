#include "stdafx.h"
#include "GameResult.h"
#include <string.h>
#include "dwrite.h"
#include "Player.h"

CGameResult::CGameResult(void)
{
	m_pRenderTarget = nullptr;
	m_DWriteFactory = nullptr;

	m_TitleTextFormat = nullptr;
	m_PlayerNameTextFormat = nullptr;
	m_PlayerScoreTextFormat = nullptr;
	m_WinnerTextFormat = nullptr;
	m_ButtonTextFormat = nullptr;

	m_pTextBrush = nullptr;
	m_pWinnerTextBrush = nullptr;
	m_pWinnerBoxBrush = nullptr;
	m_pButtonBrush = nullptr;

	m_pGoldBrush = nullptr;
	m_pTrashBrush = nullptr;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		 m_TileBrush[i] = nullptr;
	}
	
	m_Title = L"";
	m_WinnerText = L"";
	m_ButtonText = L"";

	for (int i = 0; i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerScore[i] = 0;
	}
	m_Winner = MO_NOBODY;

	m_ButtonMouseOver = false;
}


CGameResult::~CGameResult(void)
{
	SafeRelease(m_DWriteFactory);

	SafeRelease(m_TitleTextFormat);
	SafeRelease(m_PlayerNameTextFormat);
	SafeRelease(m_PlayerScoreTextFormat);
	SafeRelease(m_WinnerTextFormat);
	SafeRelease(m_ButtonTextFormat);

	SafeRelease(m_pTextBrush);
	SafeRelease(m_pWinnerTextBrush);
	SafeRelease(m_pWinnerBoxBrush);
	SafeRelease(m_pButtonBrush);
}


void CGameResult::Render()
{
	D2D1_RECT_F		rectElement, textPosition;
	D2D1_POINT_2F	pos;
	CPlayer* tempPlayer = nullptr;

	//winner background part
	pos.x = m_StartPosition.width + SC_RT_HORIZONTAL_MARGIN;
	pos.y = m_StartPosition.height + SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * static_cast<int> (m_Winner) );

	rectElement = D2D1::Rect(
		pos.x, 
		pos.y, 
		CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_StartPosition.width - m_HorizontalMargin, 
		pos.y + m_PlayerBoxHeight);

	m_pRenderTarget->FillRectangle(rectElement, m_pWinnerBoxBrush);

	//winner text part
	pos.x = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width -  m_StartPosition.width - m_HorizontalMargin - m_WinnerTextWidth;

	textPosition = D2D1::Rect(
		pos.x, 
		pos.y, 
		pos.x + m_WinnerTextWidth, 
		pos.y + m_PlayerBoxHeight);

	m_pRenderTarget->DrawText(
		m_WinnerText.c_str(),
		m_WinnerText.length(),
		m_WinnerTextFormat,
		textPosition,
		m_pTextBrush
		);

	//title part
	pos.x =  m_StartPosition.width + SC_RT_HORIZONTAL_MARGIN;
	pos.y =  m_StartPosition.height + SC_RT_VERTICAL_MARGIN;

	textPosition = D2D1::Rect(pos.x, pos.y, pos.x + m_SceneTitleWidth, pos.y + m_SceneTitleHeight);

	m_pRenderTarget->DrawText(
		m_Title.c_str(),
		m_Title.length(),
		m_TitleTextFormat,
		textPosition,
		m_pTextBrush
		);

	//player part
	for (int i = 0; i < CGameData::GetInstance()->GetplayerNumber(); ++i)
	{
		tempPlayer = CGameData::GetInstance()->GetPlayerPtrByTurn(i);

		pos.x =  m_StartPosition.width + SC_RT_HORIZONTAL_MARGIN;
		pos.y =  m_StartPosition.height + SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i);
		
		/*
		//player name back ground
		rectElement = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_PlayerNameTextWidth, 
			pos.y + m_PlayerNameTextHeight);

		m_pRenderTarget->FillRectangle(rectElement, m_pButtonBrush);
		*/

		//player name
		textPosition = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_PlayerNameTextWidth, 
			pos.y + m_PlayerNameTextHeight);

		m_pRenderTarget->DrawText(
			tempPlayer->GetPlayerName().c_str(),
			tempPlayer->GetPlayerName().length(),
			m_PlayerNameTextFormat,
			textPosition,
			m_pTextBrush
			);

		//tile part
		for (int tileCount = 0; tileCount < tempPlayer->GetPlayerItemNumber(MO_NOTHING); ++tileCount)
		{
			pos.x =  m_StartPosition.width + SC_RT_HORIZONTAL_MARGIN + m_PlayerNameTextWidth
				+ m_PlayerTileMargin //필요하면 추가 margin 포함시킬 것
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * (tileCount / 2) );
			pos.y =  m_StartPosition.height + SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i) 
				+ m_PlayerTileMargin 
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * (tileCount % 2) );

			rectElement = D2D1::Rect(
				pos.x, 
				pos.y, 
				pos.x + m_PlayerTileSize, 
				pos.y + m_PlayerTileSize);

			m_pRenderTarget->FillRectangle(
				rectElement, 
				CGameData::GetInstance()->GetPlayerPtrByTurn(i)->GetPlayerBrush()
			);
		}

		//gold part
		for (int goldCount = 0; goldCount < tempPlayer->GetPlayerItemNumber(MO_GOLD); ++goldCount)
		{
			pos.x =  m_StartPosition.width + SC_RT_HORIZONTAL_MARGIN + m_PlayerNameTextWidth
				+ m_PlayerTileMargin //필요하면 추가 margin 포함시킬 것
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * goldCount);
			pos.y =  m_StartPosition.height + SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i) 
				+ m_PlayerTileMargin
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * 2 );

			rectElement = D2D1::Rect(
				pos.x, 
				pos.y, 
				pos.x + m_PlayerTileSize, 
				pos.y + m_PlayerTileSize);

			m_pRenderTarget->FillRectangle(rectElement, m_pGoldBrush);
		}

		//trash part
		for (int trachCount = 0; trachCount < tempPlayer->GetPlayerItemNumber(MO_TRASH); ++trachCount)
		{
			pos.x =  m_StartPosition.width + SC_RT_HORIZONTAL_MARGIN + m_PlayerNameTextWidth
				+ m_PlayerTileMargin //필요하면 추가 margin 포함시킬 것
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * trachCount);
			pos.y =  m_StartPosition.height + SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i) 
				+ m_PlayerTileMargin
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * 3 );

			rectElement = D2D1::Rect(
				pos.x, 
				pos.y, 
				pos.x + m_PlayerTileSize, 
				pos.y + m_PlayerTileSize);

			m_pRenderTarget->FillRectangle(rectElement, m_pTrashBrush);
		}

		//player score
		std::wstring playerScore = std::to_wstring(m_PlayerScore[i]);

		pos.x =  m_StartPosition.width + SC_RT_HORIZONTAL_MARGIN + m_PlayerScorePosition;
		pos.y =  m_StartPosition.height + SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i);

		textPosition = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_playerScoreWidth, 
			pos.y + m_PlayerBoxHeight);

		m_pRenderTarget->DrawText(
			playerScore.c_str(),
			playerScore.length(),
			m_PlayerScoreTextFormat,
			textPosition,
			m_pTextBrush
			);
	}
	
	//button part
	pos.x = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_StartPosition.width - m_HorizontalMargin - m_ButtonWidth;
	pos.y = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height - m_StartPosition.height - m_VerticalMargin - m_ButtonHeight;
	
	if (m_ButtonMouseOver)
	{
		rectElement = D2D1::Rect(
			pos.x, 
			pos.y, 
			pos.x + m_ButtonWidth, 
			pos.y + m_ButtonHeight);

		m_pRenderTarget->FillRectangle(rectElement, m_pButtonBrush);
	}
	
	textPosition = D2D1::Rect(
		pos.x, 
		pos.y, 
		pos.x + m_ButtonWidth, 
		pos.y + m_ButtonHeight);

	m_pRenderTarget->DrawText(
		m_ButtonText.c_str(),
		m_ButtonText.length(),
		m_ButtonTextFormat,
		textPosition,
		m_pTextBrush
		);
}

bool CGameResult::Init()
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

void CGameResult::ResizeClient()
{
	//화면 크기 조절
	CalcStartPosition();
	SetObjectSize();
	RefreshTextSize();
}

void CGameResult::CalcStartPosition()
{
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	/*	현재 화면의 왼쪽 상단을 기준점으로 사용 */
	D2D1_SIZE_F centerPosition = m_pRenderTarget->GetSize();
	
	centerPosition.width /= 2;
	centerPosition.height /= 2;

	m_StartPosition.width = centerPosition.width - (WINDOW_WIDTH * tempScale) / 2;
	m_StartPosition.height = centerPosition.height - (WINDOW_HEIGHT * tempScale) / 2;
}

void CGameResult::SetObjectSize()
{
	/*	현재 렌더러에 저장된 화면 스케일에 맞춰서 
	렌더링 할 때 사용된 오브젝트들 크기 조정 */
	
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_HorizontalMargin = tempScale * SC_RT_HORIZONTAL_MARGIN;
	m_VerticalMargin = tempScale * SC_RT_VERTICAL_MARGIN;
	m_VoidSpace = tempScale * SC_RT_VOID_SPACE;

	m_SceneTitleTextSize = tempScale * SC_RT_TITLE_TEXT_SIZE;
	m_SceneTitleHeight = tempScale * SC_RT_TITLE_HEIGHT;
	m_SceneTitleWidth = tempScale * SC_RT_TITLE_WIDTH;

	m_PlayerBoxHeight = tempScale * SC_RT_PLAYER_BOX_HEIGHT;

	m_PlayerNameTextSize = tempScale * SC_RT_PLAYER_NAME_TEXT_SIZE;
	m_PlayerNameTextWidth = tempScale * SC_RT_PLAYER_NAME_TEXT_WIDTH;
	m_PlayerNameTextHeight = tempScale * SC_RT_PLAYER_NAME_TEXT_HEIGHT;

	m_PlayerScoreTextSize = tempScale * SC_RT_PLAYER_SCORE_TEXT_SIZE;
	m_PlayerScorePosition = tempScale * SC_RT_PLAYER_SCORE_POSITION;
	m_playerScoreWidth = tempScale * SC_RT_PLAYER_SCORE_WIDTH;

	m_PlayerTileSize = tempScale * SC_RT_PLAYER_TILE_SIZE;
	m_PlayerTileMargin = tempScale * SC_RT_PLAYER_TILE_MARGIN;

	m_WinnerTextSize = tempScale * SC_RT_WINNER_TEXT_SIZE;
	m_WinnerTextWidth = tempScale * SC_RT_WINNER_TEXT_WIDTH;

	m_ButtonWidth = tempScale * SC_RT_BUTTON_WIDTH;
	m_ButtonHeight = tempScale * SC_RT_BUTTON_HEIGHT;
	m_ButtonTextMagin = tempScale * SC_RT_BUTTON_TEXT_MARGIN;
	m_ButtonTextSize = tempScale * SC_RT_BUTTON_TEXT_SIZE;
}

void CGameResult::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_TitleTextFormat);
	SafeRelease(m_PlayerNameTextFormat);
	SafeRelease(m_PlayerScoreTextFormat);
	SafeRelease(m_WinnerTextFormat);
	SafeRelease(m_ButtonTextFormat);

	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_SceneTitleTextSize,
		L"ko",
		&m_TitleTextFormat
		);
	assert(SUCCEEDED(hr) );
	
	hr = m_TitleTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	assert(SUCCEEDED(hr) );

	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerNameTextSize,
		L"ko",
		&m_PlayerNameTextFormat
		);
	assert(SUCCEEDED(hr) );
	
	hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	assert(SUCCEEDED(hr) );

	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerScoreTextSize,
		L"ko",
		&m_PlayerScoreTextFormat
		);
	assert(SUCCEEDED(hr) );
	
	hr = m_PlayerScoreTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr) );

	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_WinnerTextSize,
		L"ko",
		&m_WinnerTextFormat
		);
	assert(SUCCEEDED(hr) );
	
	hr = m_WinnerTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr) );

	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_ButtonTextSize,
		L"ko",
		&m_ButtonTextFormat
		);
	assert(SUCCEEDED(hr) );

	hr = m_ButtonTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr) );

	hr = m_ButtonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr) );
}

bool CGameResult::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pButtonBrush);
		assert(SUCCEEDED(hr) );
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pTextBrush);
		assert(SUCCEEDED(hr) );
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWinnerTextBrush);
		assert(SUCCEEDED(hr) );
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SeaShell), &m_pWinnerBoxBrush);
		assert(SUCCEEDED(hr) );
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gold), &m_pGoldBrush);
		assert(SUCCEEDED(hr) );
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DimGray), &m_pTrashBrush);
		assert(SUCCEEDED(hr) );

		hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_DWriteFactory)
            );
		assert(SUCCEEDED(hr) );

		SetObjectSize();
		RefreshTextSize();

		m_Title = L"RESULT";
		m_WinnerText = L"WINNER";
		m_ButtonText = L"FINISH";
	}

	return true;
}


void CGameResult::CalculateScore()
{
	CPlayer* tempPlayer = nullptr;

	for (int turn = 0; turn < CGameData::GetInstance()->GetplayerNumber(); ++turn)
	{
		tempPlayer = CGameData::GetInstance()->GetPlayerPtrByTurn(turn);

		int	totalScore = tempPlayer->GetPlayerItemNumber(MO_NOTHING) * SC_RT_SCORE_TILE
							+ tempPlayer->GetPlayerItemNumber(MO_GOLD) * SC_RT_SCORE_GOLD
							+ tempPlayer->GetPlayerItemNumber(MO_TRASH) * SC_RT_SCORE_TRASH;
		
		m_PlayerScore[turn] = totalScore;
	}
}

void CGameResult::DecideWinner()
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

void CGameResult::InitMouseOver()
{
	m_ButtonMouseOver = false;
}

void CGameResult::SetMouseOver()
{
	m_ButtonMouseOver = true;
}

RECT CGameResult::GetGetButtonPosition()
{
	RECT buttonPosition;

	buttonPosition.left = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_HorizontalMargin - m_ButtonWidth;
	buttonPosition.right = buttonPosition.left + m_ButtonWidth;
	buttonPosition.top = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height - m_VerticalMargin - m_ButtonHeight;
	buttonPosition.bottom = buttonPosition.top + m_ButtonHeight;

	return buttonPosition;
}