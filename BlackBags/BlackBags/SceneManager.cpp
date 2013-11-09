#include "stdafx.h"
#include "SceneManager.h"
#include "OpeningScene.h"
#include "MainScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "SettingScene.h"
#include "CreditScene.h"


CSceneManager::CSceneManager(void)
{
	m_CurrentScene = new COpeningScene();
}


CSceneManager::~CSceneManager(void)
{
}

void CSceneManager::Update( Coordinate mouseCoordinate )
{
	SceneName tempName = m_CurrentScene->Update( mouseCoordinate );

	if (m_CurrentScene->getCurrentScene() != tempName)
	{
		SafeRelease(m_CurrentScene);
		switch ( tempName )
		{
		case SC_OPENING:
			m_CurrentScene = new COpeningScene();
			break;
		case SC_MAIN:
			m_CurrentScene = new CMainScene();
			break;
		case SC_PLAY:
			m_CurrentScene = new CPlayScene();
			break;
		case SC_RESULT:
			m_CurrentScene = new CResultScene();
			break;
		case SC_SETTING:
			m_CurrentScene = new CSettingScene();
			break;
		case SC_CREDIT:
			m_CurrentScene = new CCreditScene();
			break;
		default:
			break;
		}
	}
}
