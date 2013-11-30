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
	/*	finish button을 클릭하면 메인으로 넘어가고 
		그렇지 않은 경우에는 현재 scene 유지 */

	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
	}
	CGameData::GetInstance()->SetCurrentScene(SC_MAIN);
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

		PlayBGM();
	}

	return true;
}

void CResultScene::PlayBGM()
{
	//나중에 재생할 음악 선택하는 인자 넣는 형식으로 수정할 것
	CSoundRenderer::GetInstance()->PlayBGM();
}

void CResultScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}