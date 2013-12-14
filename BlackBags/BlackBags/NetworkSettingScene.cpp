#include "stdafx.h"
#include "NetworkSettingScene.h"
#include "NetworkManager.h"

CNetworkSettingScene::CNetworkSettingScene(void)
{
	m_SceneStatus = SC_NETWORK_SETTING;
}


CNetworkSettingScene::~CNetworkSettingScene(void)
{
}

bool CNetworkSettingScene::Init()
{
	CNetworkManager::GetInstance()->Connect();

	m_SettingMenu = new CSettingMenu();

	if (m_SettingMenu == nullptr || !m_SettingMenu->Init() )
	{
		return false;
	}

	AddObject(m_SettingMenu);
	
	PlayBGM();

	return true;
}