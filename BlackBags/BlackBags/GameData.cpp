#include "stdafx.h"
#include "GameData.h"

CGameData* CGameData::m_pInstance = nullptr;
CGameData::CGameData(void)
{
	m_PlayerData[0].m_Id = 0;
	m_PlayerData[0].m_PlayerName = "Jake Kim";

	m_PlayerData[1].m_Id = 1;
	m_PlayerData[1].m_PlayerName = "Cassie Kim";

	m_PlayerData[2].m_Id = 2;
	m_PlayerData[2].m_PlayerName = "Donald Kim";

	m_PlayerData[3].m_Id = 3;
	m_PlayerData[3].m_PlayerName = "Lucy Kim";

	for (int i = 0;i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerData[i].m_MyTile = 0;
		m_PlayerData[i].m_MyGold = 0;
		m_PlayerData[i].m_MyTrash = 0;
	}

	SetMapSize(0, 0);

	m_PlaySceneTimerFlag = false;
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
	SafeDelete(m_pInstance);
}

void CGameData::IncreasePlayerTileNumber( MO_OWNER playerId )
{
	assert(MO_NOBODY != playerId);
	++m_PlayerData[playerId].m_MyTile;
}

void CGameData::IncreasePlayerGoldNumber( int playerId )
{
	assert(MO_NOBODY != playerId);
	++m_PlayerData[playerId].m_MyGold;

}

void CGameData::IncreasePlayerTrashNumber( int playerId )
{
	assert(MO_NOBODY != playerId);
	++m_PlayerData[playerId].m_MyTrash;
}


void CGameData::SetMapSize(int x, int y)
{
	m_MapSize.m_Height = y;
	m_MapSize.m_Width = x;
}

int CGameData::GetPlayerTileNumber( int playerId )
{
	assert(MO_NOBODY!=playerId);
	return m_PlayerData[playerId].m_MyTile;
}

int CGameData::GetPlayerGoldNumber( int playerId )
{
	assert(MO_NOBODY!=playerId);
	return m_PlayerData[playerId].m_MyGold;
}
int CGameData::GetPlayerTrashNumber( int playerId )
{
	assert(MO_NOBODY!=playerId);
	return m_PlayerData[playerId].m_MyTrash;
}