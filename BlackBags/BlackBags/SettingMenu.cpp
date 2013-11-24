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

	CreateResource();

	// ��ư �ʱⰪ���� ������
	m_MapSelect[0].m_ButtonText = L"5 X 5";
	m_MapSelect[1].m_ButtonText = L"8 X 7";
	m_MapSelect[2].m_ButtonText = L"9 X 8";
	m_MapSelect[3].m_ButtonText = L"10 X 9";

	m_PlayerSelect[0].m_PlayerId = 0;
	m_PlayerSelect[1].m_PlayerId = 1;
	m_PlayerSelect[2].m_PlayerId = 2;
	m_PlayerSelect[3].m_PlayerId = 3;

}


CSettingMenu::~CSettingMenu(void)
{
}

// ȭ�� ���� ���������� �׸��� ���� ���� ���� ����
void CSettingMenu::CalcStartPosition()
{
	D2D1_SIZE_F Position = m_pRenderTarget->GetSize();

	Position.height /= 4;
	Position.width /= 4;

	m_StartPosition = Position;
}

// SettingMenu���� �����ϴ� ��ü���� ũ�⸦ �����Ѵ�
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
		m_MapSelect[j].m_ButtonHeight = CurrentScale * SC_DEFAULT_SELECT_MAP_BUTTON_WIDTH;
	}
}

void CSettingMenu::ResizeClient()
{
	//ȭ�� ũ�� ����
	CalcStartPosition();
	SetObjectSize();
	RefreshTextSize();
}

void CSettingMenu::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pButtonBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pUnselectedTextBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pSelectedTextBrush);

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
		CalcStartPosition();
		SetObjectSize();
		RefreshTextSize();
	}
}

void CSettingMenu::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_PlayerSelectTextFormat);
	SafeRelease(m_MapSelectTextFormat);

	// PlayerSelect â���� �ٲ�
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

	// MapSelect â�� TextFormat�� �ٲ�
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
}

void CSettingMenu::Render()
{
	//���ڸ� ���� �׸���.
	D2D1_RECT_F		rectElement, textPosition;
	D2D1_POINT_2F	pos;

	//ĳ���� ����â ����
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		pos.x = m_StartPosition.width + ( (i - 1) * m_PlayerSelect[i].m_ButtonWidth);
		pos.y = m_StartPosition.height;

		rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_PlayerSelect[i].m_ButtonWidth, pos.y + m_PlayerSelect[i].m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_PlayerSelectTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

		if (m_PlayerSelect[i].m_IsSelected)
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

	// �� ����â ����
	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		D2D1_RECT_F		rectElement, textPosition;
		D2D1_POINT_2F	pos;

		pos.x = m_StartPosition.width + ( (j - 1) * m_MapSelect[j].m_ButtonWidth);
		pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * 2;

		rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_MapSelect[j].m_ButtonWidth, pos.y + m_MapSelect[j].m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_MapSelectTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

		if (m_MapSelect[j].m_IsSelected)
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
}