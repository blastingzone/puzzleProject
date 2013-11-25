#include "stdafx.h"
#include "SettingScene.h"


CSettingScene::CSettingScene(void)
{
	m_SceneStatus = SC_SETTING;
	m_SettingMenu = new CSettingMenu();

	AddObject(m_SettingMenu);
}


CSettingScene::~CSettingScene(void)
{
	RemoveObject();
	SafeDelete(m_SettingMenu);
}

void CSettingScene::EventHandle(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// 조심해!!!!!!!
	// if문 빼야 합니다
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		///////////////////////////////////////////////////////////////////////////
		// 조심해!! 하드코딩으로 데이터 넣어주도록 했습니다
		// 테스트 끝나면 바꿔야합니다
		///////////////////////////////////////////////////////////////////////////
		CGameData::GetInstance()->SetMapSize(8,7);
		CGameData::GetInstance()->SetPlayerNumber(4);
		CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
}

void CSettingScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//아래를 지우고 자동 완성 기능을 구현합니다.
	int idx = 0;

	D2D1_SIZE_F startPosition = m_SettingMenu->GetStartPosition();
	D2D1_SIZE_F playerButton = m_SettingMenu->GetPlayerSelectButtonSize();
	D2D1_SIZE_F mapButton = m_SettingMenu->GetMapSelectButtonSize();

	if (mouseCoordinate.m_PosX > (startPosition.width - playerButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_PLAYER_NUM - 1) * playerButton.width))
	{
		if (mouseCoordinate.m_PosY > startPosition.height
			&& mouseCoordinate.m_PosY < (startPosition.height + playerButton.height))
		{
			m_SettingMenu->InitMouseOver();
			idx = static_cast<int>((mouseCoordinate.m_PosX) / playerButton.width);
			m_SettingMenu->SetPlayerMouseOver(idx);
		}
	}

	if (mouseCoordinate.m_PosX > (startPosition.width - mapButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_MAPSIZE_NUM - 1) * mapButton.width))
	{
		if (mouseCoordinate.m_PosY > (startPosition.height + 2 * playerButton.height)
			&& mouseCoordinate.m_PosY < (startPosition.height + 2 * playerButton.height + mapButton.height))
		{
			m_SettingMenu->InitMouseOver();
			idx = static_cast<int>((mouseCoordinate.m_PosX) / mapButton.width);
			m_SettingMenu->SetMapMouseOver(idx);
		}
	}
}

void CSettingScene::Render()
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
