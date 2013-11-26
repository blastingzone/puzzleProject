#include "stdafx.h"
#include "ResultScene.h"
#include "GameResult.h"

CResultScene::CResultScene(void)
{
	m_GameResult = nullptr;

	m_GameResult = new CGameResult();

	//SM9: INIT같은 것은 생성자 밖에서..
	if (!m_GameResult->Init() )
	{
		GameTerminate();
	}
	else
	{
		m_SceneStatus = SC_RESULT;
		AddObject(m_GameResult);
	}
}

CResultScene::~CResultScene(void)
{
	RemoveObject();
	SafeDelete(m_GameResult);
}

void CResultScene::EventHandle(Coordinate mouseCoordinate)
{
	/*	finish button을 클릭하면 메인으로 넘어가고 
		그렇지 않은 경우에는 현재 scene 유지 */

	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene(SC_OPENING);
	}
	CGameData::GetInstance()->SetCurrentScene(SC_OPENING);
}

void CResultScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//finish button mouse over 기능 구현
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
