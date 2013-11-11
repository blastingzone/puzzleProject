#include "stdafx.h"
#include "CreditScene.h"


CCreditScene::CCreditScene(void)
{
}


CCreditScene::~CCreditScene(void)
{
}

SceneName CCreditScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// !!!!!!!
	// if문 빼야 합니다
	// 제일 바깥쪽 return 도요
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_OPENING;
	return SC_OPENING;
}