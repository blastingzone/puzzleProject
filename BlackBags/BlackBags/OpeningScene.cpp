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