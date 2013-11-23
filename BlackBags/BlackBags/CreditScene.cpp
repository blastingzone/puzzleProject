#include "stdafx.h"
#include "CreditScene.h"


CCreditScene::CCreditScene(void)
{
}


CCreditScene::~CCreditScene(void)
{
}

void CCreditScene::EventHandle(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// !!!!!!!
	// if문 빼야 합니다
	// 제일 바깥쪽 return 도요
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
}

void CCreditScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//아래를 지우고 자동 완성 기능을 구현합니다.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}

void CCreditScene::Render()
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