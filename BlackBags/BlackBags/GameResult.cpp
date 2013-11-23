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
	m_PlayerPointTextFormat = nullptr;
	m_WinnerTextFormat = nullptr;
	m_ButtonTextFormat = nullptr;

	m_pTextBrush = nullptr;
	m_pWinnerTextBrush = nullptr;
	m_pWinnerBoxBrush = nullptr;
	m_pButtonBrush = nullptr;

	m_Title = L"";
	m_Winner = L"";
	m_Button = L"";

	//���߿� gameData���� ���� ���� �ɷ� ����
	m_Player1 = L"";
	m_Player2 = L"";
	m_Player3 = L"";
	m_Player4 = L"";
	m_Player1Point = L"";
	m_Player2Point = L"";
	m_Player3Point = L"";
	m_Player4Point = L"";
}


CGameResult::~CGameResult(void)
{
}


void CGameResult::Render()
{
	D2D1_RECT_F		rectElement, textPosition;
	D2D1_POINT_2F	pos;

	//winner background
	int winnerIdx = 2; //player3 ���߿� �������� �޵��� ������ ��

	pos.x = SC_RT_HORIZONTAL_MARGIN;
	pos.y = SC_RT_VERTICAL_MARGIN + m_SceneTitleHeight + m_VoidSpace + (m_PlayerBoxHeight * winnerIdx);

	rectElement = D2D1::Rect(
		pos.x, 
		pos.y, 
		CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - SC_RT_HORIZONTAL_MARGIN, 
		pos.y + m_PlayerBoxHeight);

	m_pRenderTarget->FillRectangle(rectElement, m_pWinnerBoxBrush);

	//RESULT
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

	for (int i = 0; i < MAX_PLAYER_NUM; ++i) //�÷��̾� �� 4���� ���� �� ����� ��
	{
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



	}
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
	m_PlayerPointTextSize = tempScale * SC_RT_PLAYER_POINT_TEXT_SIZE;
	m_PlayerTileSize = tempScale * SC_RT_PLAYER_TILE_SIZE;
	m_PlayerBoxHeight = tempScale * SC_RT_PLAYER_BOX_HEIGHT;

	m_WinnerTextSize = tempScale * SC_RT_WINNER_TEXT_SIZE;

	m_ButtonWidth = tempScale * SC_RT_BUTTON_WIDTH;
	m_ButtonHeight = tempScale * SC_RT_BUTTON_HEIGHT;
	m_ButtonTextMagin = tempScale * SC_RT_BUTTON_TEXT_SIZE;
	m_ButtonTextSize = tempScale * SC_RT_BUTTON_TEXT_MARGIN;
}

void CGameResult::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_TitleTextFormat);
	SafeRelease(m_PlayerNameTextFormat);
	SafeRelease(m_PlayerPointTextFormat);
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
		m_PlayerPointTextSize,
		L"ko",
		&m_PlayerPointTextFormat
		);

	if (SUCCEEDED(hr) )
		hr = m_PlayerPointTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
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
		hr = m_ButtonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Lime), &m_pWinnerBoxBrush);

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
			m_Winner = L"WINNER";
			m_Button = L"FINISH";

			//���߿� gameData���� ���� ���� �ɷ� ����
			m_Player1 = L"PLAYER1";
			m_Player2 = L"PLAYER2";
			m_Player3 = L"PLAYER3";
			m_Player4 = L"PLAYER4";
			m_Player1Point = L"20";
			m_Player2Point = L"11";
			m_Player3Point = L"13";
			m_Player4Point = L"64";

			return true;
		}
	}

	return false;
}