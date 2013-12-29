#include "stdafx.h"
#include "NetworkResultScene.h"
#include "NetworkGameResult.h"
#include "NetworkManager.h"

CNetworkResultScene::CNetworkResultScene(void)
{
	m_GameResult = nullptr;
	m_SceneStatus = SC_RESULT;

	ButtonPosition.bottom = 0;
	ButtonPosition.top = 0;
	ButtonPosition.left = 0;
	ButtonPosition.right = 0;
}


CNetworkResultScene::~CNetworkResultScene(void)
{
	RemoveObject();
	SafeDelete(m_GameResult);
}

void CNetworkResultScene::EventHandle(Coordinate mouseCoordinate)
{
	/*	finish button�� Ŭ���ϸ� �������� �Ѿ�� 
		�׷��� ���� ��쿡�� ���� scene ���� */

	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
	}
	CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
}

void CNetworkResultScene::MouseOver(Coordinate mouseCoordinate)
{
	m_GameResult->InitMouseOver();

	if (mouseCoordinate.m_PosX > ButtonPosition.left
		&& mouseCoordinate.m_PosX < ButtonPosition.right
		&& mouseCoordinate.m_PosY > ButtonPosition.top
		&& mouseCoordinate.m_PosY < ButtonPosition.bottom)
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
	ButtonPosition = m_GameResult->GetGetButtonPosition();
}

void CNetworkResultScene::PlayBGM()
{
	CSoundRenderer::GetInstance()->PlayBGM(BGM_RESULT);
}

void CNetworkResultScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}