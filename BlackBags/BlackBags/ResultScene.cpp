#include "stdafx.h"
#include "ResultScene.h"
#include "GameResult.h"

CResultScene::CResultScene(void)
{
	m_GameResult = nullptr;
	m_SceneStatus = SC_RESULT;

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
		CGameData::GetInstance()->SetCurrentScene(SC_OPENING);
	}
	CGameData::GetInstance()->SetCurrentScene(SC_OPENING);
}

void CResultScene::MouseOver(Coordinate mouseCoordinate)
{
	//������!!
	//finish button mouse over ��� ����
	if (mouseCoordinate.m_PosX > 0)
	{

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
	}

	return true;
}