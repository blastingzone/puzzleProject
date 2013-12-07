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
	/*	플레이어에 대한 포인터들을 확인하면서 할당되어 있지 않으면 생성하고
		이미 존재하면 초기화가 필요한 내부 변수만 초기화 */
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i] == nullptr)
		{
			m_PlayerData[i] = new CPlayer();
			m_PlayerData[i]->Init(MO_OWNER(i) );
		}
		else
		{
			m_PlayerData[i]->ResetValues();
		}
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

/*
ID2D1SolidColorBrush* CGameData::GetPlayerBrush(int turn)
{
	assert(turn >= 0 && turn < m_PlayerNumber);

	return m_PlayerData[turn]->GetPlayerBrush;
}

ID2D1Bitmap* CGameData::GetPlayerFace(int turn)
{
	assert(turn >= 0 && turn < m_PlayerNumber);

	return m_PlayerData[turn]->GetPlayerFace;
}

ID2D1Bitmap* CGameData::GetPlayerBox(int turn)
{
	assert(turn >= 0 && turn < m_PlayerNumber);

	return m_PlayerData[turn]->GetPlayerBox;
}

ID2D1Bitmap* CGameData::GetPlayerWaitingBox(int turn)
{
	assert(turn >= 0 && turn < m_PlayerNumber);

	return m_PlayerData[turn]->GetPlayerWaitingBox;
}

//최경욱 조심해!
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
*/

CPlayer* CGameData::GetPlayerPtrByTurn(int turn)
{
	assert(turn >= 0 && turn < MAX_PLAYER_NUM);

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if (m_PlayerData[i]->GetPlayerTurn() == turn)
		{
			return m_PlayerData[i];
		}
	}

	assert(false);
}

void CGameData::SetPlayerTurn(int idx, int playerTurn)	
{ 
	assert(idx >= 0 && idx < MAX_PLAYER_NUM);

	m_PlayerData[idx]->SetPlayerTurn(playerTurn); 
}

int CGameData::GetPlayerTurn(int idx)
{ 
	assert(idx >= 0 && idx < MAX_PLAYER_NUM);

	return m_PlayerData[idx]->GetPlayerTurn(); 
}

void CGameData::SetPlayerMask(int mask)
{
	m_PlayerMask = mask;
	createPlayer();
}

bool CGameData::GetPlayerCreatedFlag(int idx)
{
	assert(idx >= 0 && idx < MAX_PLAYER_NUM);

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