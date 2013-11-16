#include "stdafx.h"
#include "MainMenu.h"
#include <string.h>


CMainMenu::CMainMenu(void)
{
	m_pRenderTarget = nullptr;

	m_DWriteFactory = nullptr;
	m_pTextBrush = nullptr;
	m_TextFormat = nullptr;
}


CMainMenu::~CMainMenu(void)
{
}

void CMainMenu::Render()
{
	//D2D1_RECT_F		rectElement;
	//D2D1_POINT_2F	m_pos;

	/*
	draw text
	*/
}

void CMainMenu::Init()
{
	//자원 생성
	CreateResource();
	SetObjectSize();
	ResizeClient();
}

void CMainMenu::ResizeClient()
{
	//화면 크기 조절
	CalcStartPosition();
	SetObjectSize();
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

	m_MenuButtonWidth = tempScale * DEFAULT_MENU_BUTTON_WIDTH;
	m_MenuButtonHeight = tempScale * DEFAULT_MENU_BUTTON_HEIGHT;
}

bool CMainMenu::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		//hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(7.0f/255, 104.0f/255, 172.0f/255), &m_pMenuButtonBrush);
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkMagenta), &m_pMenuButtonBrush);

		if (SUCCEEDED(hr) )
			return true;
	}

	DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED
		,__uuidof(m_DWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_DWriteFactory)
		);

	m_DWriteFactory->CreateTextFormat(L"NIRMALA"
		, NULL
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, 20.0f
		, L"ko"
		, &m_TextFormat);

	m_TextNewGame = "NEW GAME";
	m_TextNetwork = "NETWORK";
	m_TextCredits = "CREDITS";
	m_TextCinematicMovie = "CINEMATIC TRAILER";
	m_TextExit = "EXIT";

	return false;
}