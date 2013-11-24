#include "stdafx.h"
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
	for (int i = 0; i < CGameData::GetInstance()->GetplayerNumber(); ++i) //�÷��̾� �� 4���� ���� �� ����� ��
	{
		//player name
		pos.x = SC_RT_HORIZONTAL_MARGIN;
		pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * i);

		textPosition = D2D1::Rect(
			pos.x, 
			pos.y, 
			CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - SC_RT_HORIZONTAL_MARGIN, 
			pos.y + m_PlayerBoxHeight);

		m_pRenderTarget->DrawText(
			CGameData::GetInstance()->GetPlayerName(i).c_str(),
			CGameData::GetInstance()->GetPlayerName(i).length(),
			m_PlayerNameTextFormat,
			textPosition,
			m_pTextBrush
			);

		//////////////////////////
		/*	tile + gold + trash */
		//////////////////////////

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

void CGameResult::Init()
{
	//�ְ�� ������!!
	//�̷����� �������� ���� return false�ϰ�, �� �Լ��� ȣ���� ������ ���α׷� ���� ó�����ش�.
	//�ڿ� ����
	if (!CreateResource() )
	{
		//������!!
		//���� �޽��� ���� �� ��
	}
	ResizeClient();

	CalculateScore();
	DecideWinner();
}

void CGameResult::ResizeClient()
{
	//ȭ�� ũ�� ����
	SetObjectSize();
	RefreshTextSize();
}

void CGameResult::SetObjectSize()
{
	/*	���� �������� ����� ȭ�� �����Ͽ� ���缭 
	������ �� �� ���� ������Ʈ�� ũ�� ���� */
	
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_HorizontalMargin = tempScale * SC_RT_HORIZONTAL_MARGIN;
	m_VerticalMargin = tempScale * SC_RT_VERTICAL_MARGIN;
	m_VoidSpace = tempScale * SC_RT_VOID_SPACE;

	m_SceneTitleTextSize = tempScale * SC_RT_TITLE_TEXT_SIZE;
	m_SceneTitleHeight = tempScale * SC_RT_TITLE_HEIGHT;
	m_SceneTitleWidth = tempScale * SC_RT_TITLE_WIDTH;

	m_PlayerNameTextSize = tempScale * SC_RT_PLAYER_NAME_TEXT_SIZE;
	m_PlayerScoreTextSize = tempScale * SC_RT_PLAYER_SCORE_TEXT_SIZE;
	m_PlayerScorePosition = tempScale * SC_RT_PLAYER_SCORE_POSITION;
	m_playerScoreWidth = tempScale * SC_RT_PLAYER_SCORE_WIDTH;
	m_PlayerTileSize = tempScale * SC_RT_PLAYER_TILE_SIZE;
	m_PlayerBoxHeight = tempScale * SC_RT_PLAYER_BOX_HEIGHT;

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
		hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

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

void CGameResult::SetMouseOver()
{
	//��ư ���� ���� ���� ��ư ���� true�� ����
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
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::MediumVioletRed), &m_pWinnerBoxBrush);

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

			return true;
		}
	}

	return false;
}


void CGameResult::CalculateScore()
{
	for (int playerId = 0; playerId < CGameData::GetInstance()->GetplayerNumber(); ++playerId)
	{
		int	totalScore = CGameData::GetInstance()->GetPlayerTileNumber(playerId) * 2
							+ CGameData::GetInstance()->GetPlayerGoldNumber(playerId) * 5
							- CGameData::GetInstance()->GetPlayerTrashNumber(playerId) * (-10);
		
		m_PlayerScore[playerId] = totalScore;
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
