#include "stdafx.h"
#include "SceneManager.h"

#include "OpeningScene.h"
#include "MainScene.h"
#include "SettingScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "CreditScene.h"


CSceneManager::CSceneManager(void)
{
	CGameData::GetInstance()->SetCurrentScene(SC_OPENING);
	m_CurrentScene = nullptr;
}


CSceneManager::~CSceneManager(void)
{
	SafeDelete(m_CurrentScene);
}

void CSceneManager::EventHandle( Coordinate mouseCoordinate )
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->EventHandle( mouseCoordinate );
	}
}

void CSceneManager::Update()
{
	SceneName tempName = CGameData::GetInstance()->GetCurrentScene();

	if (m_CurrentScene->GetCurrentScene() != tempName)
	{
		ChangeScene(tempName);
	}
}

void CSceneManager::ChangeScene(const SceneName& newScene)
{
	/*	현재 scene을 해제하고 적절한 다음 scene을 생성한다 */
	SafeDelete( m_CurrentScene );
	m_CurrentScene = nullptr;

	switch ( newScene )
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

void CSceneManager::MouseOver(Coordinate mouseCoordinate) 
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->MouseOver(mouseCoordinate);
	}
}

void CSceneManager::ResizeClientSize()
{
	m_CurrentScene->ResizeClient();
}

void CSceneManager::Render()
{
	m_CurrentScene->Render();
}