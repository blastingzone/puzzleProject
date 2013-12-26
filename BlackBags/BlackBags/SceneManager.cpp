#include "stdafx.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "OpeningScene.h"
#include "MainScene.h"
#include "SettingScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
#include "CreditScene.h"
#include "GameData.h"
#include "NetworkSettingScene.h"
#include "NetworkPlayScene.h"


CSceneManager::CSceneManager(HWND hWnd)
{
	m_CurrentScene = nullptr;
	m_Hwnd = NULL;
	m_Hwnd = hWnd;
	
}

CSceneManager::~CSceneManager(void)
{
	SafeDelete(m_CurrentScene);
}

bool CSceneManager::Init()
{
	CGameData::GetInstance()->Init();
	CGameData::GetInstance()->SetCurrentScene(SC_OPENING);

	m_CurrentScene = new COpeningScene();

	if (m_CurrentScene != nullptr && CGameData::GetInstance()->GetCurrentScene() == SC_OPENING)
	{
		m_CurrentScene->Init();
		return true;
	}
	return false;
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
	m_CurrentScene->StopBGM();
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
		CGameData::GetInstance()->Init();
		break;
	case SC_CREDIT:
		m_CurrentScene = new CCreditScene();
		break;
	case SC_EXIT:
		PostMessage(m_Hwnd, WM_DESTROY, NULL, NULL);
		return;
	case SC_NETWORK_SETTING:
		if (!CNetworkManager::GetInstance()->Init() )
		{
			PostMessage(m_Hwnd, WM_DESTROY, NULL, NULL);
		}
		m_CurrentScene = new CNetworkSettingScene();
		CGameData::GetInstance()->Init();
		break;
	case SC_NETWORK_PLAY:
		m_CurrentScene = new CNetworkPlayScene();
		break;
	default:
		break;
	}

	if (!m_CurrentScene->Init() )
	{
		PostMessage(m_Hwnd, WM_DESTROY, NULL, NULL);
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

//고친 부분
void CSceneManager::Render()
{
	if (m_CurrentScene != nullptr)
	{
		if (m_CurrentScene->GetCurrentScene() == SC_OPENING || m_CurrentScene->GetCurrentScene() == SC_CREDIT)
		{
			m_CurrentScene->Render();
		}
		else
		{
			CRenderer::GetInstance()->Begin();
			CRenderer::GetInstance()->Clear();
			m_CurrentScene->Render();
			CRenderer::GetInstance()->End();
		}
	}
}