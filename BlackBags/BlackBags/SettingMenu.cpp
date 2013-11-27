#include "stdafx.h"
#include "SettingMenu.h"
#include "dwrite.h"

CSettingMenu::CSettingMenu(void)
{
	m_pUnselectedTextBrush = nullptr;
	m_pSelectedTextBrush = nullptr;
	m_pButtonBrush = nullptr;
	m_pMapBackgroundBrush = nullptr;
	m_pMapSelectedBackgroundBrush = nullptr;

	// 버튼 초기값들을 설정함
	m_MapSelect[0].m_ButtonText = L"5 X 5";
	m_MapSelect[0].m_GameDataMapSizeHeight = 5;
	m_MapSelect[0].m_GameDataMapSizeWidth = 5;

	m_MapSelect[1].m_ButtonText = L"8 X 7";
	m_MapSelect[1].m_GameDataMapSizeHeight = 8;
	m_MapSelect[1].m_GameDataMapSizeWidth = 7;

	m_MapSelect[2].m_ButtonText = L"9 X 8";
	m_MapSelect[2].m_GameDataMapSizeHeight = 9;
	m_MapSelect[2].m_GameDataMapSizeWidth = 8;

	m_MapSelect[3].m_ButtonText = L"10 X 9";
	m_MapSelect[3].m_GameDataMapSizeHeight = 10;
	m_MapSelect[3].m_GameDataMapSizeWidth = 9;

	m_PlayerSelect[0].m_PlayerId = 0;
	m_PlayerSelect[1].m_PlayerId = 1;
	m_PlayerSelect[2].m_PlayerId = 2;
	m_PlayerSelect[3].m_PlayerId = 3;
}


CSettingMenu::~CSettingMenu(void)
{
}

bool CSettingMenu::Init()
{
	if (!CreateResource() )
	{
		return false;
	}
	
	return true;
}

// 화면 왼쪽 위에서부터 그리기 위해 시작 지점 설정
void CSettingMenu::CalcStartPosition()
{
	D2D1_SIZE_F Position = m_pRenderTarget->GetSize();

	Position.height /= 4;
	Position.width /= 4;

	m_StartPosition = Position;
}

// SettingMenu에서 렌더하는 객체들의 크기를 조정한다
void CSettingMenu::SetObjectSize()
{
	float CurrentScale = CRenderer::GetInstance()->GetDisplayScale();

	m_PlayerSelectTextSize = CurrentScale * SC_DEFAULT_SELECT_PLAYER_TEXT_SIZE;
	m_PlayerSelectTextMargin = CurrentScale * SC_DEFAULT_SELECT_PLAYER_TEXT_MARGIN;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_PlayerSelect[i].m_ButtonWidth = CurrentScale * SC_DEFAULT_SELECT_PLAYER_BUTTON_WIDTH;
		m_PlayerSelect[i].m_ButtonHeight = CurrentScale * SC_DEFAULT_SELECT_PLAYER_BUTTON_HEIGHT;
	}

	m_MapSelectTextSize = CurrentScale * SC_DEFAULT_SELECT_MAP_TEXT_SIZE;
	m_MapSelectTextMargin = CurrentScale * SC_DEFAULT_SELECT_MAP_TEXT_MARGIN;

	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		m_MapSelect[j].m_ButtonWidth = CurrentScale * SC_DEFAULT_SELECT_MAP_BUTTON_WIDTH;
		m_MapSelect[j].m_ButtonHeight = CurrentScale * SC_DEFAULT_SELECT_MAP_BUTTON_HEIGHT;
	}

	m_NextButton.m_ButtonHeight = CurrentScale * SC_DEFAULT_SELECT_MAP_BUTTON_HEIGHT;
	m_NextButton.m_ButtonWidth = CurrentScale * SC_DEFAULT_SELECT_MAP_BUTTON_WIDTH;
	m_NextButtonTextSize = CurrentScale * SC_DEFAULT_SELECT_NEXT_TEXT_SIZE;
	m_NextButtonTextMargin = CurrentScale * SC_DEFAULT_SELECT_NEXT_TEXT_MARGIN;
}

void CSettingMenu::ResizeClient()
{
	//화면 크기 조절
	CalcStartPosition();
	SetObjectSize();
	RefreshTextSize();
}

bool CSettingMenu::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pButtonBrush);

		/* Player별 마우스 오버 및 선택시 색상 */
		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_1_)), &m_PlayerSelect[0].m_pSelectedBackgroundBrush);
		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_1_)), &m_PlayerSelect[0].m_pBackgroundBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_2_)), &m_PlayerSelect[1].m_pSelectedBackgroundBrush);
		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_2_)), &m_PlayerSelect[1].m_pBackgroundBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_3_)), &m_PlayerSelect[2].m_pSelectedBackgroundBrush);
		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_3_)), &m_PlayerSelect[2].m_pBackgroundBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_4_)), &m_PlayerSelect[3].m_pSelectedBackgroundBrush);
		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_4_)), &m_PlayerSelect[3].m_pBackgroundBrush);
		
		/* Player별 마우스 오버 및 선택시 색상 */


		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pUnselectedTextBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pSelectedTextBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::RosyBrown), &m_pMapBackgroundBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlanchedAlmond), &m_pMapSelectedBackgroundBrush);

		if (SUCCEEDED(hr) )
			hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_DWriteFactory)
			);

		if (!SUCCEEDED(hr) )
			return false;

		CalcStartPosition();
		SetObjectSize();
		RefreshTextSize();
	}

	return true;
}

void CSettingMenu::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_PlayerSelectTextFormat);
	SafeRelease(m_MapSelectTextFormat);
	SafeRelease(m_NextButtonTextFormat);

	// PlayerSelect 창부터 바꿈
	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerSelectTextSize,
		L"ko",
		&m_PlayerSelectTextFormat
		);

	if (SUCCEEDED(hr) )
		hr = m_PlayerSelectTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// MapSelect 창의 TextFormat도 바꿈
	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerSelectTextSize,
		L"ko",
		&m_MapSelectTextFormat
		);

	if (SUCCEEDED(hr) )
		hr = m_MapSelectTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// NextButton TextFormat 생성
	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_NextButtonTextSize,
		L"ko",
		&m_NextButtonTextFormat
		);

	if (SUCCEEDED(hr) )
		hr = m_NextButtonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void CSettingMenu::Render()
{
	//상자를 먼저 그린다.
	D2D1_RECT_F		rectElement, textPosition;
	D2D1_POINT_2F	pos;

	//캐릭터 선택창 렌더
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		pos.x = m_StartPosition.width + ( (i - 1) * m_PlayerSelect[i].m_ButtonWidth);
		pos.y = m_StartPosition.height;

		rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_PlayerSelect[i].m_ButtonWidth, pos.y + m_PlayerSelect[i].m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_PlayerSelectTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

		if (m_PlayerSelect[i].m_IsMouseOver || m_PlayerSelect[i].m_IsSelected)
		{
			m_pRenderTarget->FillRectangle(rectElement, m_PlayerSelect[i].m_pSelectedBackgroundBrush);

			m_pRenderTarget->DrawText(
				CGameData::GetInstance()->GetPlayerName(m_PlayerSelect[i].m_PlayerId).c_str(),
				CGameData::GetInstance()->GetPlayerName(m_PlayerSelect[i].m_PlayerId).length(),
				m_PlayerSelectTextFormat,
				textPosition,
				m_pSelectedTextBrush
				);
		}
		else
		{
			m_pRenderTarget->DrawText(
				CGameData::GetInstance()->GetPlayerName(m_PlayerSelect[i].m_PlayerId).c_str(),
				CGameData::GetInstance()->GetPlayerName(m_PlayerSelect[i].m_PlayerId).length(),
				m_PlayerSelectTextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
	}

	// 맵 선택창 렌더
	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		D2D1_RECT_F		rectElement, textPosition;
		D2D1_POINT_2F	pos;

		pos.x = m_StartPosition.width + ( (j - 1) * m_MapSelect[j].m_ButtonWidth);
		pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * 2;

		rectElement = D2D1::Rect( pos.x,
			pos.y,
			pos.x + m_MapSelect[j].m_ButtonWidth,
			pos.y + m_MapSelect[j].m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_MapSelectTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

		if (m_MapSelect[j].m_IsMouseOver || m_MapSelect[j].m_IsSelected)
		{
			m_pRenderTarget->FillRectangle(rectElement, m_pMapSelectedBackgroundBrush);

			m_pRenderTarget->DrawText(
				m_MapSelect[j].m_ButtonText.c_str(),
				m_MapSelect[j].m_ButtonText.length(),
				m_MapSelectTextFormat,
				textPosition,
				m_pSelectedTextBrush
				);
		}
		else
		{
			m_pRenderTarget->DrawText(
				m_MapSelect[j].m_ButtonText.c_str(),
				m_MapSelect[j].m_ButtonText.length(),
				m_MapSelectTextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
	}

	//NextButton 조건이 부합하면 렌더
	if (m_NextButton.m_IsPossible)
	{
		D2D1_RECT_F		rectElement, textPosition;
		D2D1_POINT_2F	pos;

		pos.x = m_StartPosition.width + ( (MAX_MAPSIZE_NUM - 2) * m_MapSelect[0].m_ButtonWidth);
		pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * 4;

		rectElement = D2D1::Rect( pos.x,
			pos.y,
			pos.x + m_NextButton.m_ButtonWidth,
			pos.y + m_NextButton.m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_NextButtonTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

		m_pRenderTarget->FillRectangle(rectElement, m_pMapSelectedBackgroundBrush);

		m_pRenderTarget->DrawText(
			m_NextButton.m_ButtonText.c_str(),
			m_NextButton.m_ButtonText.length(),
			m_NextButtonTextFormat,
			textPosition,
			m_pSelectedTextBrush
			);
	}
}

D2D1_SIZE_F CSettingMenu::GetPlayerSelectButtonSize()
{
	D2D1_SIZE_F ButtonSize;

	ButtonSize.height = m_PlayerSelect[0].m_ButtonHeight;
	ButtonSize.width = m_PlayerSelect[0].m_ButtonWidth;

	return ButtonSize;
}

D2D1_SIZE_F CSettingMenu:: GetMapSelectButtonSize()
{
	D2D1_SIZE_F ButtonSize;

	ButtonSize.height = m_MapSelect[0].m_ButtonHeight;
	ButtonSize.width = m_MapSelect[0].m_ButtonWidth;

	return ButtonSize;
}

D2D1_SIZE_F CSettingMenu::GetNextButtonSize()
{
	D2D1_SIZE_F ButtonSize;

	ButtonSize.height = m_NextButton.m_ButtonHeight;
	ButtonSize.width = m_NextButton.m_ButtonWidth;

	return ButtonSize;
}

void CSettingMenu::SetPlayerMouseOver(int idx)
{
	assert(idx < MAX_PLAYER_NUM);
	m_PlayerSelect[idx].m_IsMouseOver = true;
}

void CSettingMenu::SetMapMouseOver(int idx)
{
	assert(idx < MAX_MAPSIZE_NUM);
	m_MapSelect[idx].m_IsMouseOver = true;
}

void CSettingMenu::InitMouseOver()
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_PlayerSelect[i].m_IsMouseOver = false;
	}

	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		m_MapSelect[j].m_IsMouseOver = false;
	}
}

void CSettingMenu::InitMapSelected()
{
	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		m_MapSelect[j].m_IsSelected = false;
	}
}