#include "stdafx.h"
#include "NetworkResultScene.h"
#include "NetworkGameResult.h"
#include "NetworkManager.h"

CNetworkResultScene::CNetworkResultScene(void)
{
	m_GameResult = nullptr;
	m_SceneStatus = SC_NETWORK_RESULT;

	m_ButtonPosition.bottom = 0;
	m_ButtonPosition.top = 0;
	m_ButtonPosition.left = 0;
	m_ButtonPosition.right = 0;
}

CNetworkResultScene::~CNetworkResultScene(void)
{
	RemoveObject();
	SafeDelete(m_GameResult);
}

void CNetworkResultScene::EventHandle(Coordinate mouseCoordinate)
{
	/*	finish button을 클릭하면 메인으로 넘어가고 
		그렇지 않은 경우에는 현재 scene 유지 */

	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
	}
	CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
}

void CNetworkResultScene::MouseOver(Coordinate mouseCoordinate)
{
	m_GameResult->InitMouseOver();

	if (mouseCoordinate.m_PosX > m_ButtonPosition.left
		&& mouseCoordinate.m_PosX < m_ButtonPosition.right
		&& mouseCoordinate.m_PosY > m_ButtonPosition.top
		&& mouseCoordinate.m_PosY < m_ButtonPosition.bottom)
	{
		m_GameResult->SetMouseOver();
	}
}

void CNetworkResultScene::Render()
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

bool CNetworkResultScene::Init()
{
	m_GameResult = new CNetworkGameResult();

	if ( m_GameResult == nullptr || !m_GameResult->Init() )
	{
		return false;
	}
	else
	{
		AddObject(m_GameResult);
		SetButtonPosition();
		PlayBGM();
	}

	return true;
}

void CNetworkResultScene::ResizeClient()
{
	for (auto iter: m_Object)
	{
		iter->ResizeClient();
	}

	SetButtonPosition();
}

void CNetworkResultScene::SetButtonPosition()
{
	m_ButtonPosition = m_GameResult->GetGetButtonPosition();
}

void CNetworkResultScene::PlayBGM()
{
	CSoundRenderer::GetInstance()->PlayBGM(BGM_RESULT);
}

void CNetworkResultScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}