#include "stdafx.h"
#include "MainScene.h"


CMainScene::CMainScene(void)
{
	Init();
	m_SceneStatus = SC_MAIN;

	AddObject(m_MainMenu);
}


CMainScene::~CMainScene(void)
{
	RemoveObject();
	SafeDelete(m_MainMenu);
}

void CMainScene::Init()
{
	m_MainMenu = new CMainMenu();
	m_MainMenu->Init();
}

SceneName CMainScene::Update(Coordinate mouseCoordinate)
{
	/* 각각의 메뉴 버튼 선택에 따라 return되는 다음 scene이 달라짐 */
	int idx = 0;

	if (mouseCoordinate.m_PosX > m_MainMenu->GetStartPosition().width - m_MainMenu->GetButtonWidth() )
	{
		idx = static_cast<int> ( (mouseCoordinate.m_PosY - m_MainMenu->GetStartPosition().height) / m_MainMenu->GetButtonHeight() );
	}
	else
	{
		idx = 5;
	}

	switch(idx)
	{
	case 0:
		return SC_SETTING;
		break;
	case 1:
		break;
	case 2:
		return SC_CREDIT;
		break;
	case 3:
		return SC_OPENING;
		break;
	case 4:
		//종료 메시지 발생 시킬 것
		break;
	default:
		break;
	}

	return SC_MAIN;
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