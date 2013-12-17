#include "stdafx.h"
#include "NetworkSettingScene.h"
#include "NetworkManager.h"
#include "PacketType.h"

CNetworkSettingScene::CNetworkSettingScene(void)
{
	m_SceneStatus = SC_NETWORK_SETTING;

	m_SelectedPlayerNumber = 0;
	m_SelectedMapIndex = -1;
}


CNetworkSettingScene::~CNetworkSettingScene(void)
{
	RemoveObject();
	SafeDelete(m_SettingMenu);
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

			// 조심해!
			// 이건 네트워크 매니저한테서 받아오는 게 더 정확할 것 같은데?
			// 일단 선택된 캐릭터인지 확인해야 한다.
			if ( !m_SettingMenu->GetPlayerSelected(idx) ) // && (m_SelectedPlayerNumber < MAX_PLAYER_NUM) <<< 이걸 체크하는 이유가 뭘까?
			{
				CSoundRenderer::GetInstance()->PlaySE_Select();
				characterSelectedByClient.mCharacterId = idx;

				//내가 고른 캐릭터를 패킷에 담아서 발송!
				if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&characterSelectedByClient, characterSelectedByClient.mSize) )
				{
					CNetworkManager::GetInstance()->PostSendMessage();
				}
			}

			// 내 캐릭터일 경우에만 취소시킬 수 있고 그 외에는 무시하게 만든다
			else if (m_SettingMenu->GetIsMineFlag(idx) ) // m_SettingMenu->GetPlayerSelected(idx) && 는 중복 체크
			{
				characterSelectedByClient.mCharacterId = -1;

				if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&characterSelectedByClient, characterSelectedByClient.mSize) )
				{
					CNetworkManager::GetInstance()->PostSendMessage();
				}
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
			CSoundRenderer::GetInstance()->PlaySE_Select();

			
			MapRequest mapRequest;
			mapRequest.mMapIdx = idx;
			
			//클라이언트아이디를 먼저 얻었어야 함.
			mapRequest.mPlayerId = CNetworkManager::GetInstance()->GetClientId();

			m_SettingMenu->InitMapSelected();
			
			if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&mapRequest, mapRequest.mSize) )
			{
				CNetworkManager::GetInstance()->PostSendMessage();
			}
			
			//m_SettingMenu->SetMapSelected(idx);
			//m_SelectedMapIndex = idx;
		}
	}

	// GameStartButton 클릭시
// 	if (CheckGameStartCondition())
// 	{
// 		m_SettingMenu->SetNextButtonPossible();
// 	}
// 	else
// 	{
// 		m_SettingMenu->SetNextButtonImpossible();
// 	}

	if (mouseCoordinate.m_PosX > startPosition.width + (MAX_MAPSIZE_NUM - 2) * nextButton.width
		&& mouseCoordinate.m_PosX < startPosition.width + (MAX_MAPSIZE_NUM - 1) * nextButton.width
		&& CheckGameStartCondition())
	{
		if (mouseCoordinate.m_PosY > startPosition.height + (SC_S_DEFAULT_NEXT_BUTTON_Y_POSITION_SCALE-1) * nextButton.height
			&& mouseCoordinate.m_PosY > startPosition.height + SC_S_DEFAULT_NEXT_BUTTON_Y_POSITION_SCALE * nextButton.height)
		{
			//send packet
			GameStartRequest startRequest;

			startRequest.mStart = true;

			if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&startRequest, startRequest.mSize) )
			{
				CNetworkManager::GetInstance()->PostSendMessage();
			}
		}
	}
}

void CNetworkSettingScene::GoNextScene()
{
	//시작 조건 확인시에 이미 값이 들어가있다.
	//m_SelectedMapIndex =  CNetworkManager::GetInstance()->GetMapIndex();
	CGameData::GetInstance()->SetMapSize(m_SettingMenu->GetMapSizeHeight(m_SelectedMapIndex)
				,m_SettingMenu->GetMapSizeWidth(m_SelectedMapIndex));
	CGameData::GetInstance()->SetPlayerNumber(m_SelectedPlayerNumber);
	CGameData::GetInstance()->SetCurrentScene( SC_NETWORK_PLAY );
	CGameData::GetInstance()->SetPlayerMask(m_SettingMenu->GetPlayerMask() );
}

void CNetworkSettingScene::MouseOver(Coordinate mouseCoordinate)
{
	int idx = 0;

	D2D1_SIZE_F startPosition = m_SettingMenu->GetStartPosition();
	D2D1_SIZE_F playerButton = m_SettingMenu->GetPlayerSelectButtonSize();
	D2D1_SIZE_F mapButton = m_SettingMenu->GetMapSelectButtonSize();

	if (mouseCoordinate.m_PosX > (startPosition.width - playerButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_PLAYER_NUM - 1) * playerButton.width))
	{
		if (mouseCoordinate.m_PosY > startPosition.height + playerButton.height * SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE
			&& mouseCoordinate.m_PosY < (startPosition.height + playerButton.height * (SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE+ 1)) )
		{
			m_SettingMenu->InitMouseOver();
			idx = static_cast<int>((mouseCoordinate.m_PosX) / playerButton.width);
			// 인덱스가 넘어가지 않게 함
			if (idx >= MAX_PLAYER_NUM)
			{
				return;
			}
			m_SettingMenu->SetPlayerMouseOver(idx);
		}
	}

	if (mouseCoordinate.m_PosX > (startPosition.width - mapButton.width)
		&& mouseCoordinate.m_PosX < (startPosition.width + (MAX_MAPSIZE_NUM - 1) * mapButton.width))
	{
		if (mouseCoordinate.m_PosY > (startPosition.height + SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE * playerButton.height)
			&& mouseCoordinate.m_PosY < (startPosition.height + SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE * playerButton.height + mapButton.height))
		{
			m_SettingMenu->InitMouseOver();
			idx = static_cast<int>((mouseCoordinate.m_PosX) / mapButton.width);
			// 인덱스가 넘어가지 않게 함
			if (idx >= MAX_MAPSIZE_NUM)
			{
				return;
			}
			m_SettingMenu->SetMapMouseOver(idx);
		}
	}
}

void CNetworkSettingScene::Render()
{
	if (CGameData::GetInstance()->GetNetworkNextSceneFlag() )
	{
		CGameData::GetInstance()->SetNetworkNextSceneFlag(false);
		GoNextScene();

		return;
	}

	for (auto iter: m_Object)
	{
		iter->Render();
#ifdef _DEBUG		
		CFPS::GetInstance()->Update();
		CFPS::GetInstance()->Render();
#endif
	}
}

bool CNetworkSettingScene::CheckGameStartCondition()
{
	m_SelectedMapIndex =  CNetworkManager::GetInstance()->GetMapIndex();
	return (m_SettingMenu->IsReady() && (m_SelectedMapIndex != -1) );
}

void CNetworkSettingScene::PlayBGM()
{
	CSoundRenderer::GetInstance()->PlayBGM(BGM_SETTING);
}

void CNetworkSettingScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}