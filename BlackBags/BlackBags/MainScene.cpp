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
	/* ������ �޴� ��ư ���ÿ� ���� return�Ǵ� ���� scene�� �޶��� */
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
		//���� �޽��� �߻� ��ų ��
		break;
	default:
		break;
	}

	return SC_MAIN;
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