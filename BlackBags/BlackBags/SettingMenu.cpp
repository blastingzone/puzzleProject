﻿#include "stdafx.h"
#include "SettingMenu.h"
#include "dwrite.h"



CSettingMenu::CSettingMenu(void)
{
	m_pUnselectedTextBrush = nullptr;
	m_pSelectedTextBrush = nullptr;
	m_pButtonBrush = nullptr;
	m_pMapBackgroundBrush = nullptr;
	m_pMapSelectedBackgroundBrush = nullptr;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_pCharacterFace[i] = nullptr;
	}

	m_PlayerMask = 0;

	// 버튼 초기값들을 설정함
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_PlayerSelect[i].m_ButtonText = L"Character %d", i;
	}

	m_PlayerSelect[0].m_ButtonText = L"Character 1";
	m_PlayerSelect[1].m_ButtonText = L"Character 2";
	m_PlayerSelect[2].m_ButtonText = L"Character 3";
	m_PlayerSelect[3].m_ButtonText = L"Character 4";

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

// SettingMenu에서 렌더하는 객체들의 크기를 조정한다 (텍스트 포맷은 따로)
void CSettingMenu::SetObjectSize()
{
	float CurrentScale = CRenderer::GetInstance()->GetDisplayScale();

	m_PlayerSelectTextSize = CurrentScale * SC_S_DEFAULT_PLAYER_TEXT_SIZE;
	m_PlayerSelectTextMargin = CurrentScale * SC_S_DEFAULT_PLAYER_TEXT_MARGIN;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_PlayerSelect[i].m_ButtonWidth = CurrentScale * SC_S_DEFAULT_PLAYER_BUTTON_WIDTH;
		m_PlayerSelect[i].m_ButtonHeight = CurrentScale * SC_S_DEFAULT_PLAYER_BUTTON_HEIGHT;
	}

	m_MapSelectTextSize = CurrentScale * SC_S_DEFAULT_MAP_TEXT_SIZE;
	m_MapSelectTextMargin = CurrentScale * SC_S_DEFAULT_MAP_TEXT_MARGIN;

	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		m_MapSelect[j].m_ButtonWidth = CurrentScale * SC_S_DEFAULT_MAP_BUTTON_WIDTH;
		m_MapSelect[j].m_ButtonHeight = CurrentScale * SC_S_DEFAULT_MAP_BUTTON_HEIGHT;
	}

	m_NextButton.m_ButtonHeight = CurrentScale * SC_S_DEFAULT_MAP_BUTTON_HEIGHT;
	m_NextButton.m_ButtonWidth = CurrentScale * SC_S_DEFAULT_MAP_BUTTON_WIDTH;

	m_NextButtonTextSize = CurrentScale * SC_S_DEFAULT_NEXT_TEXT_SIZE;
	m_NextButtonTextMargin = CurrentScale * SC_S_DEFAULT_NEXT_TEXT_MARGIN;

	m_MapTitleTextSize = CurrentScale * SC_S_DEFAULT_SUBTITLE_TEXT_SIZE;
	m_MapTitleTextMargin = CurrentScale * SC_S_DEFAULT_SUBTITLE_TEXT_MARGIN;

	m_PlayerTitleTextSize = CurrentScale * SC_S_DEFAULT_SUBTITLE_TEXT_SIZE;
	m_PlayerTitleTextMargin = CurrentScale * SC_S_DEFAULT_SUBTITLE_TEXT_MARGIN;

	m_PlayerTitle.m_LayerHeight = CurrentScale * SC_S_DEFAULT_SUBTITLE_LAYER_HEIGHT;
	m_PlayerTitle.m_LayerWidth = CurrentScale * SC_S_DEFAULT_SUBTITLE_LAYER_WIDTH;

	m_MapTitle.m_LayerHeight  = CurrentScale * SC_S_DEFAULT_SUBTITLE_LAYER_HEIGHT;
	m_MapTitle.m_LayerWidth = CurrentScale * SC_S_DEFAULT_SUBTITLE_LAYER_WIDTH;

	m_SettingTitle.m_LayerHeight = CurrentScale * SC_S_DEFAULT_MAINTITLE_LAYER_HEIGHT;
	m_SettingTitle.m_LayerWidth = CurrentScale * SC_S_DEFAULT_MAINTITLE_LAYER_WIDTH;

	m_SettingTitleTextMargin = CurrentScale * SC_S_DEFAULT_MAINTITLE_TEXT_MARGIN;
	m_SettingTitleTextSize = CurrentScale * SC_S_DEFAULT_MAINTITLE_TEXT_SIZE;

	m_PortraitWidth = CurrentScale * SC_S_DEFAULT_PORTRAIT_WIDTH;
	m_PortraitHeight = CurrentScale * SC_S_DEFAULT_PORTRAIT_HEIGHT;
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
		assert(SUCCEEDED(hr));

		/* Player별 마우스 오버 및 선택시 색상 */

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_1_)), &m_PlayerSelect[0].m_pSelectedBackgroundBrush);

		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_1_)), &m_PlayerSelect[0].m_pBackgroundBrush);

		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_2_)), &m_PlayerSelect[1].m_pSelectedBackgroundBrush);
		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_2_)), &m_PlayerSelect[1].m_pBackgroundBrush);
		assert(SUCCEEDED(hr));


		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_3_)), &m_PlayerSelect[2].m_pSelectedBackgroundBrush);
		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_3_)), &m_PlayerSelect[2].m_pBackgroundBrush);
		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_4_)), &m_PlayerSelect[3].m_pSelectedBackgroundBrush);
		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_4_)), &m_PlayerSelect[3].m_pBackgroundBrush);
		assert(SUCCEEDED(hr));

		/* Player, 맵 버튼에서 마우스 오버 및 선택시 색상 */


		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pUnselectedTextBrush);
		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pSelectedTextBrush);
		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::RosyBrown), &m_pMapBackgroundBrush);
		assert(SUCCEEDED(hr));

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlanchedAlmond), &m_pMapSelectedBackgroundBrush);
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr) )
		{
			int i = 0;

			// 캐릭터 초상화 생성

			for (i; i < MAX_PLAYER_NUM; ++i)
			{
				int currentPlayerNum = i + 1;
				std::wstring filepath = L"Resource/Image/player" + std::to_wstring(currentPlayerNum);
				filepath.append(L".png");
				m_pCharacterFace[i] = CRenderer::GetInstance()->CreateImage(filepath, m_pCharacterFace[i]);
			}
		}

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_DWriteFactory)
			);
		assert(SUCCEEDED(hr));

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
	SafeRelease(m_SubTitleTextFormat);
	SafeRelease(m_MainTitleTextFormat);

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
	assert(SUCCEEDED(hr));

	hr = m_PlayerSelectTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr));

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
	assert(SUCCEEDED(hr));

	hr = m_MapSelectTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr));

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
	assert(SUCCEEDED(hr));

	hr = m_NextButtonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr));

	// Subtitle TextFormat 생성
	// PlayerTitle 기준으로 통일
	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerTitleTextSize,
		L"ko",
		&m_SubTitleTextFormat
		);
	assert(SUCCEEDED(hr));

	hr = m_SubTitleTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr));

	// Maintitle TextFormat 생성
	hr = m_DWriteFactory->CreateTextFormat(
		_MENU_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_THIN,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_SettingTitleTextSize,
		L"ko",
		&m_MainTitleTextFormat
		);
	assert(SUCCEEDED(hr));

	hr = m_MainTitleTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr));
}

void CSettingMenu::Render()
{
	HRESULT hr;

	//상자를 먼저 그리고 그 위에 글자를 얹는 식이다
	D2D1_RECT_F		rectElement, textPosition, CharacterPortraitPosition;
	D2D1_POINT_2F	pos;

	//Setting Title을 렌더
	pos.x = m_StartPosition.width - m_SettingTitle.m_LayerWidth;
	pos.y = m_StartPosition.height - m_SettingTitle.m_LayerHeight * 3;

	rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_SettingTitle.m_LayerWidth, pos.y + m_SettingTitle.m_LayerHeight);
	textPosition =  D2D1::Rect( rectElement.left + m_SettingTitleTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

	m_pRenderTarget->DrawText(
		m_SettingTitle.m_Title.c_str(),
		m_SettingTitle.m_Title.length(),
		m_MainTitleTextFormat,
		textPosition,
		m_pUnselectedTextBrush
		);

	// 캐릭터 선택창을 알리는 타이틀을 렌더
	// 위치선정
	pos.x = m_StartPosition.width + (-1) * m_PlayerSelect[0].m_ButtonWidth;
	pos.y = m_StartPosition.height - m_PlayerTitle.m_LayerHeight;

	rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_PlayerTitle.m_LayerWidth, pos.y + m_PlayerTitle.m_LayerHeight);
	textPosition =  D2D1::Rect( rectElement.left + m_PlayerTitleTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

	m_pRenderTarget->DrawText(
		m_PlayerTitle.m_Title.c_str(),
		m_PlayerTitle.m_Title.length(),
		m_SubTitleTextFormat,
		textPosition,
		m_pUnselectedTextBrush
		);

	// 캐릭터 선택창 y 값
	pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE;

	//캐릭터 선택창 렌더
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		pos.x = m_StartPosition.width + ( (i - 1) * m_PlayerSelect[i].m_ButtonWidth);

		rectElement = D2D1::Rect( pos.x,
			pos.y,
			pos.x + m_PlayerSelect[i].m_ButtonWidth,
			pos.y + m_PlayerSelect[i].m_ButtonHeight);

		textPosition =  D2D1::Rect( rectElement.left + m_PlayerSelectTextMargin,
			rectElement.top,
			rectElement.right,
			rectElement.bottom);
		// 캐릭터 초상화 부분
		// MouseOver 구현시 아래의 if else 문에 넣어서 조절해준다
		CharacterPortraitPosition = D2D1::Rect ( rectElement.left,
			rectElement.top - SC_S_DEFAULT_PORTRAIT_HEIGHT,
			rectElement.left + SC_S_DEFAULT_PORTRAIT_WIDTH,
			rectElement.top);

		// 마우스가 올라가거나 선택된 상태면 자신이 가진 브러쉬로 자신을 칠함
		if (m_PlayerSelect[i].m_IsMouseOver || m_PlayerSelect[i].m_IsSelected)
		{
			m_pRenderTarget->FillRectangle(rectElement, m_PlayerSelect[i].m_pSelectedBackgroundBrush);

			m_pRenderTarget->DrawText(
				m_PlayerSelect[i].m_ButtonText.c_str(),
				m_PlayerSelect[i].m_ButtonText.length(),
				m_PlayerSelectTextFormat,
				textPosition,
				m_pSelectedTextBrush
				);
		}
		else // 아니면 글자만 나옴
		{
			m_pRenderTarget->DrawText(
				m_PlayerSelect[i].m_ButtonText.c_str(),
				m_PlayerSelect[i].m_ButtonText.length(),
				m_PlayerSelectTextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
		// 캐릭터 초상화 포지션은 Character 선택창의 중심으로부터
		// 캐릭터 초상화 렌더
		m_pRenderTarget->DrawBitmap(m_pCharacterFace[i], CharacterPortraitPosition);
	}

	// 맵 선택창을 알리는 타이틀을 렌더
	pos.x = m_StartPosition.width + (-1) * m_MapSelect[0].m_ButtonWidth;
	pos.y = m_StartPosition.height + m_PlayerTitle.m_LayerHeight * (SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE - 2);

	rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_MapTitle.m_LayerWidth,
		pos.y + m_MapTitle.m_LayerHeight);
	textPosition =  D2D1::Rect( rectElement.left + m_MapTitleTextMargin,
		rectElement.top,
		rectElement.right,
		rectElement.bottom);

	m_pRenderTarget->DrawText(
		m_MapTitle.m_Title.c_str(),
		m_MapTitle.m_Title.length(),
		m_SubTitleTextFormat,
		textPosition,
		m_pUnselectedTextBrush
		);

	// 맵 선택창 y 밑으로 얼마나 내려갈지
	pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE;

	// 맵 선택창 렌더
	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		pos.x = m_StartPosition.width + ( (j - 1) * m_MapSelect[j].m_ButtonWidth);

		rectElement = D2D1::Rect( pos.x,
			pos.y,
			pos.x + m_MapSelect[j].m_ButtonWidth,
			pos.y + m_MapSelect[j].m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_MapSelectTextMargin,
			rectElement.top,
			rectElement.right,
			rectElement.bottom);

		// 맵도 마찬가지로 선택되거나 마우스가 올라가 있으면 색이 변함
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

	//NextButton 조건이 부합하면 렌더. 이 조건이 부합하지 않으면 활성화되지도 않는다(클릭불가)
	if (m_NextButton.m_IsPossible)
	{
		pos.x = m_StartPosition.width + ( (MAX_MAPSIZE_NUM - 2) * m_MapSelect[0].m_ButtonWidth);
		// 밑으로  얼마나 내려갈지
		pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_NEXT_BUTTON_Y_POSITION_SCALE;

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

// 맵을 선택했을 때, 혹은 선택한 맵을 다시 클릭했을 때 발생
void CSettingMenu::InitMapSelected()
{
	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		m_MapSelect[j].m_IsSelected = false;
	}
}

// Mask 값을 할당하여 어떤 플레이어가 선택되었는지 4비트로 알 수 있게
void CSettingMenu::SetPlayerSelected(int idx)
{
	m_PlayerSelect[idx].m_IsSelected = true;
	switch (idx)
	{
	case 0:
		m_PlayerMask += MASK_PLAYER_1;
		break;
	case 1:
		m_PlayerMask += MASK_PLAYER_2;
		break;
	case 2:
		m_PlayerMask += MASK_PLAYER_3;
		break;
	case 3:
		m_PlayerMask += MASK_PLAYER_4;
		break;
	}
}

void CSettingMenu::CancelPlayerSelected(int idx)
{
	m_PlayerSelect[idx].m_IsSelected = false;
	switch (idx)
	{
	case 0:
		m_PlayerMask -= MASK_PLAYER_1;
		break;
	case 1:
		m_PlayerMask -= MASK_PLAYER_2;
		break;
	case 2:
		m_PlayerMask -= MASK_PLAYER_3;
		break;
	case 3:
		m_PlayerMask -= MASK_PLAYER_4;
		break;
	}
}