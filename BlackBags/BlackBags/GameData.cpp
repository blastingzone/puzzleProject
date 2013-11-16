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

	Init();
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

void CGameData::SetPlayerTileNumber( int playerId, int tileNumber )
{
	m_PlayerData[playerId].m_MyTile = tileNumber;
}

void CGameData::SetPlayerGoldNumber( int playerId, int goldNumber )
{
	m_PlayerData[playerId].m_MyGold = goldNumber;
}

void CGameData::SetPlayerTrashNumber( int playerId, int trashNumber )
{
	m_PlayerData[playerId].m_MyTrash = trashNumber;
}

void CGameData::Init()
{
	for (int i = 0;i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerData[i].m_MyTile = 0;
		m_PlayerData[i].m_MyGold = 0;
		m_PlayerData[i].m_MyTrash = 0;
	}
}
