#include "stdafx.h"
#include "SettingScene.h"


CSettingScene::CSettingScene(void)
{
}


CSettingScene::~CSettingScene(void)
{
}

SceneName CSettingScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_PLAY;
	return SC_PLAY;
}