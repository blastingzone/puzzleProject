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
	// 조심해!!!!!!!
	// if문 빼야 합니다
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
	{
		///////////////////////////////////////////////////////////////////////////
		// 조심해!! 하드코딩으로 데이터 넣어주도록 했습니다
		// 테스트 끝나면 바꿔야합니다
		///////////////////////////////////////////////////////////////////////////
		CGameData::GetInstance()->SetMapSize(8,7);
		CGameData::GetInstance()->SetPlayerNumber(4);
		return SC_PLAY;
	}
	return SC_PLAY;
}