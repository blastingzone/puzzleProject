﻿#include "stdafx.h"
#include <d2d1.h>
#include "MainScene.h"
#include "MainMenu.h"

CMainScene::CMainScene(void)
{
	m_MainMenu = nullptr;

	m_MainMenu = new CMainMenu();
	if (!m_MainMenu->Init() ) //SM9: 생성자에서는 초기화만 하고 뭔가 실행하지 말 것.. 예외 발생시 핸들링을 할 수 없어서 프로그램을 죽이는 수밖에 없음.
	{
		GameTerminate();
	}

	m_SceneStatus = SC_MAIN;

	AddObject(m_MainMenu);
}


CMainScene::~CMainScene(void)
{
	RemoveObject();
	SafeDelete(m_MainMenu);
}

void CMainScene::EventHandle(Coordinate mouseCoordinate)
{
	int idx = 0;

	/*	메뉴 버튼 범위 안에서 클릭이 발생하면 해당 인덱스에 해당하는 버튼에 연결된 SceneName을 반환하고 
		그렇지 않은 경우에는 현재 scene 유지 */
	if (mouseCoordinate.m_PosX > m_MainMenu->GetStartPosition().width - m_MainMenu->GetButtonWidth() )
	{
		idx = static_cast<int> ( (mouseCoordinate.m_PosY - m_MainMenu->GetStartPosition().height) / m_MainMenu->GetButtonHeight() );

		SceneName nextScene = m_MainMenu->GetLinkedSceneName(idx);
		if (nextScene != SC_NOSCENE)
		{
			CGameData::GetInstance()->SetCurrentScene( nextScene );
		}
	}
}

void CMainScene::MouseOver(Coordinate mouseCoordinate)
{
	int idx = 0;

	/*	마우스 포인터가 메뉴 버튼 영역에 들어올 경우 그때의 버튼 idx를 계산해서 MainMenu에 넘겨주고, 해당하는 버튼이 화면에 표시되게 함 */
	if (mouseCoordinate.m_PosX > m_MainMenu->GetStartPosition().width - m_MainMenu->GetButtonWidth() 
		&& mouseCoordinate.m_PosY > m_MainMenu->GetStartPosition().height)
	{
		idx = static_cast<int> ( (mouseCoordinate.m_PosY - m_MainMenu->GetStartPosition().height) / m_MainMenu->GetButtonHeight() );

		if (idx < BUTTON_NUMBER)
		{
			m_MainMenu->SetMouseOver(idx);
		}
		else
		{
			m_MainMenu->InitMouseOver();
		}
	}
	else
	{
		m_MainMenu->InitMouseOver();
	}
}

void CMainScene::Render()
{
	for (auto iter: m_Object)
	{
		iter->Render();
#ifdef _DEBUG		
		CFPS::GetInstance()->Update();
		CFPS::GetInstance()->Render();
#endif
	}
}