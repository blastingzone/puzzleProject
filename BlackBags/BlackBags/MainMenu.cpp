#include "stdafx.h"
#include "MainMenu.h"
#include <string.h>
#include "dwrite.h"


CMainMenu::CMainMenu(void)
{
	m_pRenderTarget = nullptr;
}


CMainMenu::~CMainMenu(void)
{
}

void CMainMenu::Render()
{
	D2D1_RECT_F		rectElement;
	D2D1_POINT_2F	pos;
	
	//background
	m_pRenderTarget->DrawBitmap(CGameData::GetInstance()->GetBackgroundImage(), D2D1::RectF(0,0,m_BackgroundWidth,m_BackgroundHeight) );

	//title
	pos.x = m_StartPosition.width - (m_TitleWidth / 2);
	pos.y = m_StartPosition.height - m_TitleHeight - m_TitleMargin;

	rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_TitleWidth, pos.y + m_TitleHeight);

	m_pRenderTarget->DrawBitmap(m_pTitle, rectElement);

	//buttons
	pos.x = m_StartPosition.width - (m_MenuButtonWidth / 2);
	pos.y = m_StartPosition.height;

	for (int i = 0; i < BUTTON_NUMBER; ++i)
	{
		rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_MenuButtonWidth, pos.y + m_MenuButtonHeight);

		//mouseover에 따른 이미지 렌더
		if (m_ButtonList[i].m_MouseOver)
		{
			m_pRenderTarget->DrawBitmap(m_ButtonList[i].m_pSelected, rectElement);
		}
		else
		{
			m_pRenderTarget->DrawBitmap(m_ButtonList[i].m_pUnselected, rectElement);
		}

		//다음 버튼 그릴 위치 지정
		pos.y += m_MenuButtonHeight;
	}
}

bool CMainMenu::Init()
{
	if (!CreateResource() )
	{
		return false;
	}
	ResizeClient();
	
	return true;
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
	D2D1_SIZE_F centerPosition = m_pRenderTarget->GetSize();

	centerPosition.height /= 2;
	centerPosition.width /= 2;

	m_StartPosition.width = centerPosition.width;
	m_StartPosition.height = centerPosition.height;
}

void CMainMenu::SetObjectSize()
{
	/*	현재 렌더러에 저장된 화면 스케일에 맞춰서 
	렌더링 할 때 사용된 오브젝트들 크기 조정 */
	
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_MenuButtonWidth = tempScale * SC_M_DEFAULT_MENU_BUTTON_WIDTH;
	m_MenuButtonHeight = tempScale * SC_M_DEFAULT_MENU_BUTTON_HEIGHT;
	m_MenuMenuMargin = tempScale * SC_M_DEFAULT_MENU_MARGIN;
	m_TitleWidth = tempScale * SC_M_DEFAULT_TITLE_WIDTH;
	m_TitleHeight = tempScale * SC_M_DEFAULT_TITLE_HEIGHT;
	m_TitleMargin = tempScale * SC_M_DEFAULT_TITLE_MARGIN;
	m_BackgroundWidth = tempScale * WINDOW_WIDTH;
	m_BackgroundHeight = tempScale * WINDOW_HEIGHT;;
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

		m_pTitle = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_title1.png", m_pTitle);

		SetObjectSize();

		//new game
		m_ButtonList[0].m_pSelected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_newgame_selected.png", m_ButtonList[0].m_pSelected);
		m_ButtonList[0].m_pUnselected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_newgame.png", m_ButtonList[0].m_pUnselected);
		m_ButtonList[0].m_LinkedScene = SC_SETTING;

		//multiplay
		m_ButtonList[1].m_pSelected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_multiplay_selected.png", m_ButtonList[1].m_pSelected);
		m_ButtonList[1].m_pUnselected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_multiplay.png", m_ButtonList[1].m_pUnselected);
		m_ButtonList[1].m_LinkedScene = SC_NETWORK_NAMESETTING;

		//credit
		m_ButtonList[2].m_pSelected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_credit_selected.png", m_ButtonList[2].m_pSelected);
		m_ButtonList[2].m_pUnselected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_credit.png", m_ButtonList[2].m_pUnselected);
		m_ButtonList[2].m_LinkedScene = SC_CREDIT;

		//exit
		m_ButtonList[3].m_pSelected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_exit_selected.png", m_ButtonList[3].m_pSelected);
		m_ButtonList[3].m_pUnselected = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/MAIN_exit.png", m_ButtonList[3].m_pUnselected);
		m_ButtonList[3].m_LinkedScene = SC_EXIT;
	}

	return true;
}