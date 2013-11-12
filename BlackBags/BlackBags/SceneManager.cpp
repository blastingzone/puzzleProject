#include "stdafx.h"
#include "SceneManager.h"
#include "OpeningScene.h"
#include "MainScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "SettingScene.h"
#include "CreditScene.h"
#include "Renderer.h"


CSceneManager::CSceneManager(void)
{
	m_CurrentScene = new COpeningScene();
}


CSceneManager::~CSceneManager(void)
{
}


void CSceneManager::Update( Coordinate mouseCoordinate ) //SM9: 사실 이 함수의 의미는 ChangeScene아닌가? 
	// 게임 클라에서 Update 함수가 갖는 의미는 프레임마다 뭔가 계산해주는 함수를 의미하니까 이렇게 쓰면 헷갈리게됨.
{
	SceneName tempName = m_CurrentScene->Update( mouseCoordinate );

	if (m_CurrentScene->getCurrentScene() != tempName)
	{
		delete m_CurrentScene;
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

void CSceneManager::Release()
{
	delete m_CurrentScene;
}

void CSceneManager::ResizeClientSize()
{
	m_CurrentScene->ResizeClient();
}