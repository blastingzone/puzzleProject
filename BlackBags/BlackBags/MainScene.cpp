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
	// 조심해!!!!!!!
	// if문 빼야 합니다
	// 제일 바깥쪽 return 도요
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_SETTING;
	return SC_SETTING;
}