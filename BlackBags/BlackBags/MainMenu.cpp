#include "stdafx.h"
#include "MainMenu.h"
#include <string.h>


CMainMenu::CMainMenu(void)
{
	m_pRenderTarget = nullptr;
	m_DWriteFactory = nullptr;
	m_pUnselectedTextBrush = nullptr;
	m_pSelectedTextBrush = nullptr;
	m_TextFormat = nullptr;
}


CMainMenu::~CMainMenu(void)
{
}

void CMainMenu::Render()
{
	D2D1_RECT_F		rectElement, textPosition;
	D2D1_POINT_2F	pos;

	for (int i = 0; i < BUTTON_NUMBER; ++i)
	{
		//pos�� �޴� ��ư�� ���� ���
		pos.x = m_StartPosition.width - m_MenuButtonWidth;
		pos.y = m_StartPosition.height + (m_MenuButtonHeight * i);

		rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_MenuButtonWidth, pos.y + m_MenuButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_MenuTextMagin, rectElement.top, rectElement.right, rectElement.bottom);

		if (m_ButtonList[i].m_MouseOver)
		{
			m_pRenderTarget->FillRectangle(rectElement, m_pMenuButtonBrush);

			rectElement.left += m_MenuTextMagin;

			m_pRenderTarget->DrawText(
			m_ButtonList[i].m_ButtonText,
			m_ButtonList[i].m_StrLength,
			m_TextFormat,
			textPosition,
			m_pSelectedTextBrush
			);
		}
		else
		{
			rectElement.left -= m_MenuTextMagin;

			m_pRenderTarget->DrawText(
				m_ButtonList[i].m_ButtonText,
				m_ButtonList[i].m_StrLength,
				m_TextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
	}
}

void CMainMenu::Init()
{
	//�ڿ� ����
	if (!CreateResource() )
	{
		//������!!
		//���� �޽��� ���� �� ��
	}
	SetObjectSize();
	ResizeClient();
}

void CMainMenu::ResizeClient()
{
	//ȭ�� ũ�� ����
	CalcStartPosition();
	SetObjectSize();
}

void CMainMenu::CalcStartPosition()
{
	/*	���� ȭ���� ������ ����� ���������� ��� */
	D2D1_SIZE_F rightPosition;
	rightPosition = m_pRenderTarget->GetSize();

	rightPosition.height /= 2;

	m_StartPosition.width = rightPosition.width;
	m_StartPosition.height = rightPosition.height;
}

void CMainMenu::SetObjectSize()
{
	/*	���� �������� ����� ȭ�� �����Ͽ� ���缭 
	������ �� �� ���� ������Ʈ�� ũ�� ���� */
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_MenuButtonWidth = tempScale * DEFAULT_MENU_BUTTON_WIDTH;
	m_MenuButtonHeight = tempScale * DEFAULT_MENU_BUTTON_HEIGHT;
	m_MenuTextMagin = tempScale * DEFAULT_TEXT_MARGIN;
}

void CMainMenu::InitMouseOver()
{
	for (int i = 0; i < BUTTON_NUMBER; ++i)
	{
		m_ButtonList[i].m_MouseOver = false;
	}
}

void CMainMenu::SetMouseOver(int idx)
{
	InitMouseOver();
	// ������ ��� ��� ���� ó��
	assert(idx < BUTTON_NUMBER);
	m_ButtonList[idx].m_MouseOver = true;
}

bool CMainMenu::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		//hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(7.0f/255, 104.0f/255, 172.0f/255), &m_pMenuButtonBrush);
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pMenuButtonBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pUnselectedTextBrush);

		if (SUCCEEDED(hr) )
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pSelectedTextBrush);

		if (SUCCEEDED(hr) )
			hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_DWriteFactory)
            );

		if (SUCCEEDED(hr) )
			hr = m_DWriteFactory->CreateTextFormat(
				L"Segoe UI",                // Font family name.
				NULL,							// Font collection (NULL sets it to use the system font collection).
				DWRITE_FONT_WEIGHT_THIN,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				20.0f,
				L"ko",
				&m_TextFormat
				);

		if (SUCCEEDED(hr) )
			hr = m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		if (SUCCEEDED(hr) ){
			m_ButtonList[0].m_ButtonText = L"NEW GAME";
			m_ButtonList[0].m_StrLength  = (UINT32) wcslen(m_ButtonList[0].m_ButtonText);

			m_ButtonList[1].m_ButtonText = L"NETWORK";
			m_ButtonList[1].m_StrLength  = (UINT32) wcslen(m_ButtonList[1].m_ButtonText);

			m_ButtonList[2].m_ButtonText = L"CREDITS";
			m_ButtonList[2].m_StrLength  = (UINT32) wcslen(m_ButtonList[2].m_ButtonText);

			m_ButtonList[3].m_ButtonText = L"OPENNING";
			m_ButtonList[3].m_StrLength  = (UINT32) wcslen(m_ButtonList[3].m_ButtonText);

			m_ButtonList[4].m_ButtonText = L"EXIT";
			m_ButtonList[4].m_StrLength  = (UINT32) wcslen(m_ButtonList[4].m_ButtonText);

			return true;
		}
	}

	return false;
}