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
			// �ε����� �Ѿ�� �ʰ� ��
			if (idx >= MAX_PLAYER_NUM)
			{
				return;
			}

			CharacterRequest characterSelectedByClient;
			characterSelectedByClient.mPlayerId = CNetworkManager::GetInstance()->GetClientId();

			// ������!
			// �̰� ��Ʈ��ũ �Ŵ������׼� �޾ƿ��� �� �� ��Ȯ�� �� ������?
			// �ϴ� ���õ� ĳ�������� Ȯ���ؾ� �Ѵ�.
			if ( !m_SettingMenu->GetPlayerSelected(idx) ) // && (m_SelectedPlayerNumber < MAX_PLAYER_NUM) <<< �̰� üũ�ϴ� ������ ����?
			{
				CSoundRenderer::GetInstance()->PlaySE_Select();
				characterSelectedByClient.mCharacterId = idx;

				//���� �� ĳ���͸� ��Ŷ�� ��Ƽ� �߼�!
				if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&characterSelectedByClient, characterSelectedByClient.mSize) )
				{
					CNetworkManager::GetInstance()->PostSendMessage();
				}
			}

			// �� ĳ������ ��쿡�� ��ҽ�ų �� �ְ� �� �ܿ��� �����ϰ� �����
			else if (m_SettingMenu->GetIsMineFlag(idx) ) // m_SettingMenu->GetPlayerSelected(idx) && �� �ߺ� üũ
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
			// �ε����� �Ѿ�� �ʰ� ��
			if (idx >= MAX_MAPSIZE_NUM)
			{
				return;
			}
			CSoundRenderer::GetInstance()->PlaySE_Select();

			
			MapRequest mapRequest;
			mapRequest.mMapIdx = idx;
			
			//Ŭ���̾�Ʈ���̵� ���� ������ ��.
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

	// GameStartButton Ŭ����
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
			// �ε����� �Ѿ�� �ʰ� ��
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
			// �ε����� �Ѿ�� �ʰ� ��
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