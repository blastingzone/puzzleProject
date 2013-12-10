#include "stdafx.h"
#include "MainMenu.h"
#include <string.h>
#include "dwrite.h"


CMainMenu::CMainMenu(void)
{
	m_pRenderTarget = nullptr;

	m_DWriteFactory = nullptr;
	m_TextFormat = nullptr;

	m_pUnselectedTextBrush = nullptr;
	m_pSelectedTextBrush = nullptr;
	m_pMenuButtonBrush = nullptr;
	m_pBackgroundBrush = nullptr;
}


CMainMenu::~CMainMenu(void)
{
	//sm9: 이 내용이 호출 안될 수 있다는 말씀.
	SafeRelease(m_DWriteFactory);
	SafeRelease(m_TextFormat);

	SafeRelease(m_pUnselectedTextBrush);
	SafeRelease(m_pSelectedTextBrush);
	SafeRelease(m_pMenuButtonBrush);
	SafeRelease(m_pBackgroundBrush);
}

void CMainMenu::Render()
{
	D2D1_RECT_F		rectElement, textPosition;
	D2D1_POINT_2F	pos;
	
	m_pRenderTarget->DrawBitmap(m_pBackgroundImage,D2D1::RectF(0,0,m_BackgroundWidth,m_BackgroundHeight));

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
			m_ButtonList[i].m_ButtonText.c_str(),
			m_ButtonList[i].m_ButtonText.length(),
			m_TextFormat,
			textPosition,
			m_pSelectedTextBrush
			);
		}
		else
		{
			rectElement.left -= m_MenuTextMagin;

			m_pRenderTarget->DrawText(
				m_ButtonList[i].m_ButtonText.c_str(),
				m_ButtonList[i].m_ButtonText.length(),
				m_TextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
	}
}

bool CMainMenu::Init()
{
	if (!CreateResource() )
	{
		return false;
	}
	ResizeClient();
	m_pBackgroundImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/background_menu.png",m_pBackgroundImage);

	return true;
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
	D2D1_SIZE_F rightPosition = m_pRenderTarget->GetSize();

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
	m_BackgroundWidth = tempScale * WINDOW_WIDTH;
	m_BackgroundHeight = tempScale * WINDOW_HEIGHT;
}

void CMainMenu::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_TextFormat);

	hr = m_DWriteFactory->CreateTextFormat(
			_MENU_FONT,
			NULL,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_MenuTextSize,
			L"ko",
			&m_TextFormat
			);
	assert(SUCCEEDED(hr) );

	hr = m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	assert(SUCCEEDED(hr) );
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

SceneName CMainMenu::GetLinkedSceneName(int idx)
{
	if (idx < BUTTON_NUMBER)
	{
		return m_ButtonList[idx].m_LinkedScene;
	}
	
	return SC_NOSCENE;
}

bool CMainMenu::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pMenuButtonBrush);
		assert(SUCCEEDED(hr) );

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pUnselectedTextBrush);
		assert(SUCCEEDED(hr) );

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pSelectedTextBrush);
		assert(SUCCEEDED(hr) );

		hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_DWriteFactory)
            );

		//sm9: 라이브러리 호출에 대한 것은 assert보다 직접 에러 처리 할 것. release에서도 에러 핸들링 해야지?
		assert(SUCCEEDED(hr) );

		SetObjectSize();
		RefreshTextSize();

		m_ButtonList[0].m_ButtonText = L"NEW GAME";
		m_ButtonList[0].m_LinkedScene = SC_SETTING;

		//조심해!
		//나중에 네트워크 추가되면 씬도 추가해서 수정 할 것
		m_ButtonList[1].m_ButtonText = L"NETWORK";
		m_ButtonList[1].m_LinkedScene = SC_NOSCENE;

		m_ButtonList[2].m_ButtonText = L"CREDITS";
		m_ButtonList[2].m_LinkedScene = SC_CREDIT;

		m_ButtonList[3].m_ButtonText = L"OPENNING";
		m_ButtonList[3].m_LinkedScene = SC_OPENING;

		m_ButtonList[4].m_ButtonText = L"EXIT";
		m_ButtonList[4].m_LinkedScene = SC_EXIT;
	}

	return true;
}