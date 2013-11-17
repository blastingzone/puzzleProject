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
		//pos는 메뉴 버튼의 왼쪽 상단
		pos.x = m_StartPosition.width - m_MenuButtonWidth;
		pos.y = m_StartPosition.height + (m_MenuButtonHeight * i);

		rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_MenuButtonWidth, pos.y + m_MenuButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_MenuTextMagin, rectElement.top, rectElement.right, rectElement.bottom);

		/*	해당 버튼에 mouseOver상태가 true이면 버튼을 화면에 표시하고 그 위에 텍스트를 렌더 */
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
	//자원 생성
	if (!CreateResource() )
	{
		//조심해!!
		//종료 메시지 생성 할 것
	}
	SetObjectSize();
	ResizeClient();
}

void CMainMenu::ResizeClient()
{
	//화면 크기 조절
	CalcStartPosition();
	SetObjectSize();
	RefreshTextSize();
}

void CMainMenu::CalcStartPosition()
{
	/*	현재 화면의 오른쪽 가운데를 기준점으로 사용 */
	D2D1_SIZE_F rightPosition;
	rightPosition = m_pRenderTarget->GetSize();

	rightPosition.height /= 2;

	m_StartPosition.width = rightPosition.width;
	m_StartPosition.height = rightPosition.height;
}

void CMainMenu::SetObjectSize()
{
	/*	현재 렌더러에 저장된 화면 스케일에 맞춰서 
	렌더링 할 때 사용된 오브젝트들 크기 조정 */
	
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_MenuButtonWidth = tempScale * SC_M_DEFAULT_MENU_BUTTON_WIDTH;
	m_MenuButtonHeight = tempScale * SC_M_DEFAULT_MENU_BUTTON_HEIGHT;
	m_MenuTextMagin = tempScale * SC_M_DEFAULT_TEXT_MARGIN;
	m_MenuTextSize = tempScale * SC_M_DEFAULT_TEXT_SIZE;
}

void CMainMenu::RefreshTextSize()
{
	HRESULT hr;

	m_TextFormat->Release();

	hr = m_DWriteFactory->CreateTextFormat(
			L"Segoe UI",
			NULL,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_MenuTextSize,
			L"ko",
			&m_TextFormat
			);

	if (SUCCEEDED(hr) )
		hr = m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
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
	// 범위를 벗어난 경우 예외 처리
	assert(idx < BUTTON_NUMBER);
	m_ButtonList[idx].m_MouseOver = true;
}

bool CMainMenu::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

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

		SetObjectSize();

		if (SUCCEEDED(hr) )
			hr = m_DWriteFactory->CreateTextFormat(
				L"Segoe UI",                
				NULL,
				DWRITE_FONT_WEIGHT_THIN,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				m_MenuTextSize,
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