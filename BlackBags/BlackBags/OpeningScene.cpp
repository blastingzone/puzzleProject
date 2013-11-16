#include "stdafx.h"
#include "OpeningScene.h"


COpeningScene::COpeningScene(void)
{
}

COpeningScene::~COpeningScene(void)
{
}

SceneName COpeningScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// 조심해!!!!!!!
	// if문 빼야 합니다
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_MAIN;
	return SC_MAIN;
}

void COpeningScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//아래를 지우고 자동 완성 기능을 구현합니다.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}