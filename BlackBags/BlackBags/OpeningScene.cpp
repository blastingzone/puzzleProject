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
	// ������!!!!!!!
	// if�� ���� �մϴ�
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_MAIN;
	return SC_MAIN;
}