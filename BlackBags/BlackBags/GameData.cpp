#include "stdafx.h"
#include "GameData.h"
#include "Player.h"

CGameData* CGameData::m_pInstance = nullptr;

CGameData::CGameData(void)
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_PlayerData[i] = nullptr;
	}

	m_CurrentScene = SC_NOSCENE;

	//m_PlayerMask = 0;
}


CGameData::~CGameData(void)
{
}

CGameData* CGameData::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameData();
	}

	return m_pInstance;
}

void CGameData::ReleaseInstance()
{
	m_pInstance->Release();
	SafeDelete(m_pInstance);
}

void CGameData::Release()
{
	for (int i = 0; i < m_PlayerData.size(); ++i)
	{
		SafeDelete(m_PlayerData[i]);
	}
}

void CGameData::Init()
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i] == nullptr)
		{
			switch (i)
			{
			case 0:
				m_PlayerData[i] = new CPlayer();
				m_PlayerData[i]->SetPlayerName(L"Player1");
				m_PlayerData[i]->SetPlayerImage(L"Resource/Image/player1.png");
				m_PlayerData[i]->SetPlayerBox(L"Resource/Image/player1Box.png");
				m_PlayerData[i]->SetBrushColor(D2D1::ColorF(_COLOR_PLAYER_1_) );
				break;
			case 1:
				m_PlayerData[i] = new CPlayer();
				m_PlayerData[i]->SetPlayerName(L"Player2");
				m_PlayerData[i]->SetPlayerImage(L"Resource/Image/player2.png");
				m_PlayerData[i]->SetPlayerBox(L"Resource/Image/player2Box.png");
				m_PlayerData[i]->SetBrushColor(D2D1::ColorF(_COLOR_PLAYER_2_) );
				break;
			case 2:
				m_PlayerData[i] = new CPlayer();
				m_PlayerData[i]->SetPlayerName(L"Player3");
				m_PlayerData[i]->SetPlayerImage(L"Resource/Image/player3.png");
				m_PlayerData[i]->SetPlayerBox(L"Resource/Image/player3Box.png");
				m_PlayerData[i]->SetBrushColor(D2D1::ColorF(_COLOR_PLAYER_3_) );
				break;
			case 3:
				m_PlayerData[i] = new CPlayer();
				m_PlayerData[i]->SetPlayerName(L"Player4");
				m_PlayerData[i]->SetPlayerImage(L"Resource/Image/player4.png");
				m_PlayerData[i]->SetPlayerBox(L"Resource/Image/player4Box.png");
				m_PlayerData[i]->SetBrushColor(D2D1::ColorF(_COLOR_PLAYER_4_) );
			default:
				break;
			}
			m_PlayerData[i]->SetPlayerId(i);
		}

		m_PlayerData[i]->Init();
	}

	m_PlayerNumber = 0;
	m_PlayerMask = 0;

	SetMapSize(0, 0);

	m_PlaySceneTimerFlag = false;
}

void CGameData::SetMapSize(int x, int y)
{
	m_MapSize.m_Height = y;
	m_MapSize.m_Width = x;
}

const std::wstring& CGameData::GetPlayerName(int idx)
{ 
	return m_PlayerData[idx]->GetPlayerName(); 
}

//아래 함수들의 리턴값이 위험하다
//수정할 것
const std::wstring& CGameData::GetPlayerImage(int turn)
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i]->GetPlayerTurn() == turn)
		{
			return m_PlayerData[i]->GetPlayerImage();
		}
	}

	return L"";
}

const std::wstring& CGameData::GetPlayerBox(int turn)
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i]->GetPlayerTurn() == turn)
		{
			return m_PlayerData[i]->GetPlayerBox();
		}
	}

	return L"";
}

D2D1_COLOR_F CGameData::GetPlayerBrushColor(int turn)
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i]->GetPlayerTurn() == turn)
		{
			return m_PlayerData[i]->GetBrushColor();
		}
	}

	return D2D1::ColorF(0);
}

void CGameData::UpdatePlayerResult(int turn, MO_ITEM item)
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i]->GetPlayerTurn() == turn)
		{
			return m_PlayerData[i]->UpdatePlayerResult(item);
		}
	}
}

int CGameData::GetPlayerItemNumber(int turn, MO_ITEM item)
{
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i]->GetPlayerTurn() == turn)
		{
			return m_PlayerData[i]->GetPlayerItemNumber(item);
		}
	}

	return 0;
}

void CGameData::SetPlayerTurn(int idx, int playerTurn)	
{ 
	m_PlayerData[idx]->SetPlayerTurn(playerTurn); 
}

int CGameData::GetPlayerTurn(int idx)
{ 
	return m_PlayerData[idx]->GetPlayerTurn(); 
}

void CGameData::SetPlayerMask(int mask)
{
	m_PlayerMask = mask;
	createPlayer();
}

bool CGameData::GetPlayerCreatedFlag(int idx)
{
	return m_PlayerData[idx]->GetPlayerCreatedFlag();
}

void CGameData::createPlayer()
{
	if ( (m_PlayerMask & 0x01) == 0x1)
	{
		m_PlayerData[0]->SetPlayerCreatedFlag();
	}

	if ( (m_PlayerMask & 0x02) == 0x2)
	{
		m_PlayerData[1]->SetPlayerCreatedFlag();
	}

	if ( (m_PlayerMask & 0x04) == 0x4)
	{
		m_PlayerData[2]->SetPlayerCreatedFlag();
	}

	if ( (m_PlayerMask & 0x08) == 0x8)
	{
		m_PlayerData[3]->SetPlayerCreatedFlag();
	}
}