#include "stdafx.h"
#include "ResultScene.h"


CResultScene::CResultScene(void)
{
}


CResultScene::~CResultScene(void)
{
}

SceneName CResultScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// 조심해!!!!!!!
	// if문 빼야 합니다
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_OPENING;
	return SC_OPENING;
}

void CResultScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//아래를 지우고 자동 완성 기능을 구현합니다.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}