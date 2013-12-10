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

	assert(false);
	//sm9: 리턴 구문 없다. 반드시 넣을 것. assert는 release 모드에서는 동작하지 않는다.

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
	//sm9: 사실 아래 if 문은 for 문으로 shift하면서 루프 하나로 처리할 수 있다.
	/* 대략 이런식...
	for (int i=0, j=0x01 ; i<4 ; ++i, j <<= 1 )
	{

		if ( m_PlayerMask & j == j )
		{
			m_PlayerData[i]->SetPlayerCreatedFlag() ;
		}
	}
	*/

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