#include "stdafx.h"
#include "SettingScene.h"


CSettingScene::CSettingScene(void)
{
	m_SceneStatus = SC_SETTING;
	
	m_SelectedPlayerNumber = 0;
	m_SelectedMapIndex = -1;
}


CSettingScene::~CSettingScene(void)
{
	RemoveObject();
	SafeDelete(m_SettingMenu);
}


bool CSettingScene::Init()
{
	m_SettingMenu = new CSettingMenu();

	if (m_SettingMenu == nullptr || !m_SettingMenu->Init() )
	{
		return false;
	}

	AddObject(m_SettingMenu);
	
	return true;
}

void CSettingScene::EventHandle(Coordinate mouseCoordinate)
{
	int idx = 0;

	D2D1_SIZE_F startPosition = m_SettingMenu->GetStartPosition();
	D2D1_SIZE_F playerButton = m_SettingMenu->GetPlayerSelectButtonSize();
	D2D1_SIZE_F mapButton = m_SettingMenu->GetMapSelectButtonSize();
	D2D1_SIZE_F nextButton = m_SettingMenu->GetNextButtonSize();

	// Player Select && Cancel Selection
	if (mouseCoordinate.m_PosX > (startPosition.width - playerButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_PLAYER_NUM - 1) * playerButton.width))
	{
		if (mouseCoordinate.m_PosY > startPosition.height
			&& mouseCoordinate.m_PosY < (startPosition.height + playerButton.height))
		{
			idx = static_cast<int>((mouseCoordinate.m_PosX) / playerButton.width);
			if (!m_SettingMenu->GetPlayerSelected(idx) && (m_SelectedPlayerNumber < MAX_PLAYER_NUM) )
			{
				++m_SelectedPlayerNumber;
				m_SettingMenu->SetPlayerSelected(idx);
			}
			else if (m_SettingMenu->GetPlayerSelected(idx) )
			{
				--m_SelectedPlayerNumber;
				m_SettingMenu->CancelPlayerSelected(idx);
			}
		}
	}

	// Map Select
	if (mouseCoordinate.m_PosX > (startPosition.width - mapButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_MAPSIZE_NUM - 1) * mapButton.width))
	{
		if (mouseCoordinate.m_PosY > (startPosition.height + 2 * playerButton.height)
			&& mouseCoordinate.m_PosY < (startPosition.height + 2 * playerButton.height + mapButton.height))
		{
			idx = static_cast<int>((mouseCoordinate.m_PosX) / mapButton.width);
			m_SettingMenu->InitMapSelected();
			m_SettingMenu->SetMapSelected(idx);
			m_SelectedMapIndex = idx;
		}
	}

	if (CheckGameStartCondition())
	{
		m_SettingMenu->SetNextButtonPossible();
	}
	else
	{
		m_SettingMenu->SetNextButtonImpossible();
	}

	if (mouseCoordinate.m_PosX > startPosition.width + (MAX_MAPSIZE_NUM - 2) * nextButton.width
		&& mouseCoordinate.m_PosX < startPosition.width + (MAX_MAPSIZE_NUM - 1) * nextButton.width
		&& CheckGameStartCondition())
	{
		if (mouseCoordinate.m_PosY > startPosition.height + (3) * nextButton.height
			&& mouseCoordinate.m_PosY > startPosition.height + (4) * nextButton.height)
		{
			///////////////////////////////////////////////////////////////////////////
			// 조심해!! 
			// Player Create와 연동해야 합니다!!
			///////////////////////////////////////////////////////////////////////////
			CGameData::GetInstance()->SetMapSize(m_SettingMenu->GetMapSizeHeight(m_SelectedMapIndex)
				,m_SettingMenu->GetMapSizeWidth(m_SelectedMapIndex));
			CGameData::GetInstance()->SetPlayerNumber(m_SelectedPlayerNumber);
			CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
		}
	}
	//CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
}

void CSettingScene::MouseOver(Coordinate mouseCoordinate)
{
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

bool CSettingScene::CheckGameStartCondition()
{
	if (m_SelectedPlayerNumber > 1 && (m_SelectedMapIndex != -1) )
	{
		return true;
	}
	return false;
}
