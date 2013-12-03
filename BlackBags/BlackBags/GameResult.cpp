﻿#include "stdafx.h"
#include "GameResult.h"
#include <string.h>
#include "dwrite.h"

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

	//winner background part
	pos.x = SC_RT_HORIZONTAL_MARGIN;
	pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * static_cast<int> (m_Winner) );

	rectElement = D2D1::Rect(
		pos.x, 
		pos.y, 
		CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_HorizontalMargin, 
		pos.y + m_PlayerBoxHeight);

	m_pRenderTarget->FillRectangle(rectElement, m_pWinnerBoxBrush);

	//winner text part
	pos.x = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_HorizontalMargin - m_WinnerTextWidth;

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
	pos.x = SC_RT_HORIZONTAL_MARGIN;
	pos.y = SC_RT_VERTICAL_MARGIN;

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
		pos.x = SC_RT_HORIZONTAL_MARGIN;
		pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i);
		
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
			CGameData::GetInstance()->GetPlayerName(i).c_str(),
			CGameData::GetInstance()->GetPlayerName(i).length(),
			m_PlayerNameTextFormat,
			textPosition,
			m_pTextBrush
			);

		//tile part
		for (int tileCount = 0; tileCount < CGameData::GetInstance()->GetPlayerTileNumber(i); ++tileCount)
		{
			pos.x = SC_RT_HORIZONTAL_MARGIN + m_PlayerNameTextWidth
				+ m_PlayerTileMargin //필요하면 추가 margin 포함시킬 것
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * (tileCount / 2) );
			pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i) 
				+ m_PlayerTileMargin 
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * (tileCount % 2) );

			rectElement = D2D1::Rect(
				pos.x, 
				pos.y, 
				pos.x + m_PlayerTileSize, 
				pos.y + m_PlayerTileSize);

			m_pRenderTarget->FillRectangle(rectElement, m_TileBrush[i]);
		}

		//gold part
		for (int goldCount = 0; goldCount < CGameData::GetInstance()->GetPlayerGoldNumber(i); ++goldCount)
		{
			pos.x = SC_RT_HORIZONTAL_MARGIN + m_PlayerNameTextWidth
				+ m_PlayerTileMargin //필요하면 추가 margin 포함시킬 것
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * goldCount);
			pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i) 
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
		for (int trachCount = 0; trachCount < CGameData::GetInstance()->GetPlayerTrashNumber(i); ++trachCount)
		{
			pos.x = SC_RT_HORIZONTAL_MARGIN + m_PlayerNameTextWidth
				+ m_PlayerTileMargin //필요하면 추가 margin 포함시킬 것
				+ ( (m_PlayerTileSize + m_PlayerTileMargin) * trachCount);
			pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i) 
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

		pos.x = SC_RT_HORIZONTAL_MARGIN + m_PlayerScorePosition;
		pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i);

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
	pos.x = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_HorizontalMargin - m_ButtonWidth;
	pos.y = CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height - m_VerticalMargin - m_ButtonHeight;
	
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
	SetObjectSize();
	RefreshTextSize();
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

	if (SUCCEEDED(hr) )
		hr = m_TitleTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

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

	if (SUCCEEDED(hr) )
		hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);

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

	if (SUCCEEDED(hr) )
		hr = m_PlayerScoreTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

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

	if (SUCCEEDED(hr) )
		hr = m_WinnerTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

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

	if (SUCCEEDED(hr) )
	{
		hr = m_ButtonTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		hr = m_ButtonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

bool CGameResult::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pButtonBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pTextBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWinnerTextBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SeaShell), &m_pWinnerBoxBrush);
		
		//sm9: API 호출 뒤에는 반드시 그 결과값 HRESULT 확인하고 에러 핸들링 할 것.
		// 아래 코드 부터는 succeeded(hr)의미가 없음
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gold), &m_pGoldBrush);
		
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DimGray), &m_pTrashBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_1_), &m_TileBrush[0]);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_2_), &m_TileBrush[1]);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_3_), &m_TileBrush[2]);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_4_), &m_TileBrush[3]);

		if (SUCCEEDED(hr) )
			hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_DWriteFactory)
            );

		SetObjectSize();
		RefreshTextSize();

		if (SUCCEEDED(hr) )
		{
			m_Title = L"RESULT";
			m_WinnerText = L"WINNER";
			m_ButtonText = L"FINISH";
		}
		
		if (!SUCCEEDED(hr) )
			return false;
	}

	return true;
}


void CGameResult::CalculateScore()
{
	for (int playerId = 0; playerId < CGameData::GetInstance()->GetplayerNumber(); ++playerId)
	{
		int	totalScore = CGameData::GetInstance()->GetPlayerTileNumber(playerId) * SC_RT_SCORE_TILE
							+ CGameData::GetInstance()->GetPlayerGoldNumber(playerId) * SC_RT_SCORE_GOLD
							+ CGameData::GetInstance()->GetPlayerTrashNumber(playerId) * SC_RT_SCORE_TRASH;
		
		//sm9: GetplayerNumber()가 4가 넘을 가능성은 없을까? 
		// 지금은 MAX_PLAYER로 selected player 입력시에 체크는 하고 있는데, 만일 실수로 기능 추가하다가 입력시 체크를 빼먹을 수 있다.
		// 그래서, 방어적으로 프로그래밍 하는게 좋다. 배열 인자에 대해 assert를 사용하거나 std::array를 사용하거나..
		m_PlayerScore[playerId] = totalScore;
	}
}

void CGameResult::DecideWinner()
{
	int winnerId = 0;

	for (int i = 1; i < CGameData::GetInstance()->GetplayerNumber(); ++i)
	{
		//sm9: 이것도 마찬가지다. m_PlayerScore의 크기는 차라리 GetplayerNumber() 크기 만큼 동적으로 생성해주는게 좋다.
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