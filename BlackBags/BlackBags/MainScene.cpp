#include "stdafx.h"
#include "MainScene.h"


CMainScene::CMainScene(void)
{
}


CMainScene::~CMainScene(void)
{
	RemoveObject();
}

void	CMainScene::Init()
{
	m_MainMenu = new CMainMenu();
}

SceneName CMainScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	// ���� �ٱ��� return ����
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_SETTING;
	return SC_SETTING;
}