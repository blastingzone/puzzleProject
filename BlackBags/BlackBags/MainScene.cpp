#include "stdafx.h"
#include "MainScene.h"


CMainScene::CMainScene(void)
{
}


CMainScene::~CMainScene(void)
{
}

SceneName CMainScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	// ���� �ٱ��� return ����
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_SETTING;
	return SC_SETTING;
}