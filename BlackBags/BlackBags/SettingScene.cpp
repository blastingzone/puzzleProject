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
	{
		///////////////////////////////////////////////////////////////////////////
		// ������!! �ϵ��ڵ����� ������ �־��ֵ��� �߽��ϴ�
		// �׽�Ʈ ������ �ٲ���մϴ�
		///////////////////////////////////////////////////////////////////////////
		CGameData::GetInstance()->SetMapSize(8,7);
		CGameData::GetInstance()->SetPlayerNumber(4);
		return SC_PLAY;
	}
	return SC_PLAY;
}