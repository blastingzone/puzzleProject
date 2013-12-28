#include "stdafx.h"
#include <d2d1.h>
#include "MainScene.h"
#include "MainMenu.h"

CMainScene::CMainScene(void)
{
	m_MainMenu = nullptr;

	m_SceneStatus = SC_MAIN;
}


CMainScene::~CMainScene(void)
{
	RemoveObject();
	SafeDelete(m_MainMenu);
}

bool CMainScene::Init()
{
	m_MainMenu = new CMainMenu();

	if (!m_MainMenu->Init() || m_MainMenu == nullptr)
	{
		return false;
	}
	
	AddObject(m_MainMenu);

	PlayBGM();

	return true;
}

void CMainScene::EventHandle(Coordinate mouseCoordinate)
{
	int idx = 0;

	/*	�޴� ��ư ���� �ȿ��� Ŭ���� �߻��ϸ� �ش� �ε����� �ش��ϴ� ��ư�� ����� SceneName�� ��ȯ�ϰ� 
		�׷��� ���� ��쿡�� ���� scene ���� */
	if (mouseCoordinate.m_PosX > m_MainMenu->GetStartPosition().width - (m_MainMenu->GetButtonWidth() / 2)
		&& mouseCoordinate.m_PosX < m_MainMenu->GetStartPosition().width + (m_MainMenu->GetButtonWidth() / 2)
		&& mouseCoordinate.m_PosY > m_MainMenu->GetStartPosition().height
		&& mouseCoordinate.m_PosY < m_MainMenu->GetStartPosition().height + (m_MainMenu->GetButtonHeight() * BUTTON_NUMBER) )
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
	if (mouseCoordinate.m_PosX > m_MainMenu->GetStartPosition().width - (m_MainMenu->GetButtonWidth() / 2)
		&& mouseCoordinate.m_PosX < m_MainMenu->GetStartPosition().width + (m_MainMenu->GetButtonWidth() / 2)
		&& mouseCoordinate.m_PosY > m_MainMenu->GetStartPosition().height
		&& mouseCoordinate.m_PosY < m_MainMenu->GetStartPosition().height + (m_MainMenu->GetButtonHeight() * BUTTON_NUMBER) )
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

void CMainScene::PlayBGM()
{
	CSoundRenderer::GetInstance()->PlayBGM(BGM_MAIN);
}

void CMainScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}