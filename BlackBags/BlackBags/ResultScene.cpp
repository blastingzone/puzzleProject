#include "stdafx.h"
#include "ResultScene.h"
#include "GameResult.h"
#include "NetworkManager.h"

CResultScene::CResultScene(void)
{
	m_GameResult = nullptr;
	m_SceneStatus = SC_RESULT;

	m_ButtonPosition.bottom = 0;
	m_ButtonPosition.top = 0;
	m_ButtonPosition.left = 0;
	m_ButtonPosition.right = 0;
}

CResultScene::~CResultScene(void)
{
	RemoveObject();
	SafeDelete(m_GameResult);
}

void CResultScene::EventHandle(Coordinate mouseCoordinate)
{
	/*	finish button�� Ŭ���ϸ� �������� �Ѿ�� 
		�׷��� ���� ��쿡�� ���� scene ���� */

	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
	}
	CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
}

void CResultScene::MouseOver(Coordinate mouseCoordinate)
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

void CResultScene::Render()
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

bool CResultScene::Init()
{
	m_GameResult = new CGameResult();

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

void CResultScene::ResizeClient()
{
	for (auto iter: m_Object)
	{
		iter->ResizeClient();
	}

	SetButtonPosition();
}

void CResultScene::SetButtonPosition()
{
	m_ButtonPosition = m_GameResult->GetGetButtonPosition();
}

void CResultScene::PlayBGM()
{
	CSoundRenderer::GetInstance()->PlayBGM(BGM_RESULT);
}

void CResultScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}