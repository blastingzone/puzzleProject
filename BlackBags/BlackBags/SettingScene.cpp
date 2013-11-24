#include "stdafx.h"
#include "SettingScene.h"


CSettingScene::CSettingScene(void)
{
	m_SceneStatus = SC_SETTING;
	m_SettingMenu = new CSettingMenu();

	AddObject(m_SettingMenu);
}


CSettingScene::~CSettingScene(void)
{
	RemoveObject();
}

void CSettingScene::EventHandle(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		///////////////////////////////////////////////////////////////////////////
		// ������!! �ϵ��ڵ����� ������ �־��ֵ��� �߽��ϴ�
		// �׽�Ʈ ������ �ٲ���մϴ�
		///////////////////////////////////////////////////////////////////////////
		CGameData::GetInstance()->SetMapSize(8,7);
		CGameData::GetInstance()->SetPlayerNumber(4);
		CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
}

void CSettingScene::MouseOver(Coordinate mouseCoordinate)
{
	//������!!
	//�Ʒ��� ����� �ڵ� �ϼ� ����� �����մϴ�.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}

void CSettingScene::Render()
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
