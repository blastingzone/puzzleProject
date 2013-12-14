#include "stdafx.h"
#include "NetworkSettingScene.h"
#include "NetworkManager.h"
#include "PacketType.h"

CNetworkSettingScene::CNetworkSettingScene(void)
{
	m_SceneStatus = SC_NETWORK_SETTING;
}


CNetworkSettingScene::~CNetworkSettingScene(void)
{
}

bool CNetworkSettingScene::Init()
{
	if (!CNetworkManager::GetInstance()->Connect()
		&&!CNetworkManager::GetInstance()->IsLoginComplete())
		return false;

	CNetworkManager::GetInstance()->AskClientId();

	m_SettingMenu = new CNetworkSettingMenu();

	if (m_SettingMenu == nullptr || !m_SettingMenu->Init() )
	{
		return false;
	}

	AddObject(m_SettingMenu);
	
	PlayBGM();

	return true;
}

void CNetworkSettingScene::EventHandle(Coordinate mouseCoordinate)
{
	int idx = 0;

	D2D1_SIZE_F startPosition	= m_SettingMenu->GetStartPosition();
	D2D1_SIZE_F playerButton	= m_SettingMenu->GetPlayerSelectButtonSize();
	D2D1_SIZE_F mapButton		= m_SettingMenu->GetMapSelectButtonSize();
	D2D1_SIZE_F nextButton		= m_SettingMenu->GetNextButtonSize();

	// Player Select && Cancel Selection
	if (mouseCoordinate.m_PosX > (startPosition.width - playerButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_PLAYER_NUM - 1) * playerButton.width))
	{
		if (mouseCoordinate.m_PosY > startPosition.height + playerButton.height * SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE
			&& mouseCoordinate.m_PosY < (startPosition.height + playerButton.height * (SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE+ 1)) )
		{
			idx = static_cast<int>((mouseCoordinate.m_PosX) / playerButton.width);
			// 인덱스가 넘어가지 않게 함
			if (idx >= MAX_PLAYER_NUM)
			{
				return;
			}

			CharacterRequest characterSelectedByClient;
			characterSelectedByClient.mPlayerId = CNetworkManager::GetInstance()->GetClientId();

			// 캐릭터를 선택해서 내 걸로 만든다 조건 : IsMine이 True여야 한다
			if (!m_SettingMenu->GetPlayerSelected(idx) && (m_SelectedPlayerNumber < MAX_PLAYER_NUM) )
			{
				// 서버에 물어보고 확인한다.
// 				++m_SelectedPlayerNumber;
// 				m_SettingMenu->SetPlayerSelected(idx);
				
				characterSelectedByClient.mCharacterId = idx;
				CNetworkManager::GetInstance()->GetSendBuffer()->Write(&characterSelectedByClient, characterSelectedByClient.mSize);
			}

			// 내 캐릭터일 경우에만 취소시킬 수 있고 그 외에는 무시하게 만든다
			else if ( m_SettingMenu->GetPlayerSelected(idx) && m_SettingMenu->GetIsMineFlag(idx) )
			{
				// 서버에 물어보고 확인한다.
// 				--m_SelectedPlayerNumber;
// 				m_SettingMenu->CancelPlayerSelected(idx);
				
				characterSelectedByClient.mCharacterId = -1;
				CNetworkManager::GetInstance()->GetSendBuffer()->Write(&characterSelectedByClient, characterSelectedByClient.mSize);
			}
		}
	}

	// Map Select
	if (mouseCoordinate.m_PosX > (startPosition.width - mapButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_MAPSIZE_NUM - 1) * mapButton.width))
	{
		if (mouseCoordinate.m_PosY > (startPosition.height + SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE * playerButton.height)
			&& mouseCoordinate.m_PosY < (startPosition.height + SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE * playerButton.height + mapButton.height))
		{
			idx = static_cast<int>((mouseCoordinate.m_PosX) / mapButton.width);
			// 인덱스가 넘어가지 않게 함
			if (idx >= MAX_MAPSIZE_NUM)
			{
				return;
			}
			m_SettingMenu->InitMapSelected();
			m_SettingMenu->SetMapSelected(idx);
			m_SelectedMapIndex = idx;
		}
	}

	// GameStartButton 클릭시
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
		if (mouseCoordinate.m_PosY > startPosition.height + (SC_S_DEFAULT_NEXT_BUTTON_Y_POSITION_SCALE-1) * nextButton.height
			&& mouseCoordinate.m_PosY > startPosition.height + SC_S_DEFAULT_NEXT_BUTTON_Y_POSITION_SCALE * nextButton.height)
		{
			CGameData::GetInstance()->SetMapSize(m_SettingMenu->GetMapSizeHeight(m_SelectedMapIndex)
				,m_SettingMenu->GetMapSizeWidth(m_SelectedMapIndex));
			CGameData::GetInstance()->SetPlayerNumber(m_SelectedPlayerNumber);
			CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
			CGameData::GetInstance()->SetPlayerMask(m_SettingMenu->GetPlayerMask() );
		}
	}
}