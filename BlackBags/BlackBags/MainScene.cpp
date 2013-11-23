#include "stdafx.h"
#include <d2d1.h>
#include "MainScene.h"
#include "MainMenu.h"

CMainScene::CMainScene(void)
{
	m_MainMenu = nullptr;

	m_MainMenu = new CMainMenu();
	m_MainMenu->Init();

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

	/*	�޴� ��ư ���� �ȿ��� Ŭ���� �߻��ϸ� �ش� �ε����� �ش��ϴ� ��ư�� ����� SceneName�� ��ȯ�ϰ� 
		�׷��� ���� ��쿡�� ���� scene ���� */
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

	/*	���콺 �����Ͱ� �޴� ��ư ������ ���� ��� �׶��� ��ư idx�� ����ؼ� MainMenu�� �Ѱ��ְ�, �ش��ϴ� ��ư�� ȭ�鿡 ǥ�õǰ� �� */
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