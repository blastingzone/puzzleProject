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
	for (unsigned int i = 0; i < m_PlayerData.size(); ++i)
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

	m_hWnd = nullptr;
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

	return nullptr;
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
	for (int i = 0, j = 0x01 ; i < MAX_PLAYER_NUM ; ++i, j <<= 1 )
	{

		if ( (m_PlayerMask & j) == j )
		{
			m_PlayerData[i]->SetPlayerCreatedFlag() ;
		}
	}

}