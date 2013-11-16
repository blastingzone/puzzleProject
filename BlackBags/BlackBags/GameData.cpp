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

void CGameData::IncreasePlayerTileNumber( MO_OWNER playerId )
{
	switch (playerId)
	{
	case MO_PLAYER1:
		++m_PlayerData[MO_PLAYER1].m_MyTile;
		break;
	case MO_PLAYER2:
		++m_PlayerData[MO_PLAYER2].m_MyTile;
		break;
	case MO_PLAYER3:
		++m_PlayerData[MO_PLAYER3].m_MyTile;
		break;
	case MO_PLAYER4:
		++m_PlayerData[MO_PLAYER4].m_MyTile;
		break;
	case MO_NOBODY:
		break;
	}
}

void CGameData::IncreasePlayerGoldNumber( int playerId )
{
	switch (playerId)
	{
	case MO_PLAYER1:
		++m_PlayerData[MO_PLAYER1].m_MyGold;
		break;
	case MO_PLAYER2:
		++m_PlayerData[MO_PLAYER2].m_MyGold;
		break;
	case MO_PLAYER3:
		++m_PlayerData[MO_PLAYER3].m_MyGold;
		break;
	case MO_PLAYER4:
		++m_PlayerData[MO_PLAYER4].m_MyGold;
		break;
	case MO_NOBODY:
		break;
	}
}

void CGameData::IncreasePlayerTrashNumber( int playerId )
{
	switch (playerId)
	{
	case MO_PLAYER1:
		++m_PlayerData[MO_PLAYER1].m_MyTrash;
		break;
	case MO_PLAYER2:
		++m_PlayerData[MO_PLAYER2].m_MyTrash;
		break;
	case MO_PLAYER3:
		++m_PlayerData[MO_PLAYER3].m_MyTrash;
		break;
	case MO_PLAYER4:
		++m_PlayerData[MO_PLAYER4].m_MyTrash;
		break;
	case MO_NOBODY:
		break;
	}
}

void CGameData::Init()
{
	for (int i = 0;i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerData[i].m_MyTile = 0;
		m_PlayerData[i].m_MyGold = 0;
		m_PlayerData[i].m_MyTrash = 0;
	}

	SetMapSize(0, 0);
}

void CGameData::SetMapSize(int x, int y)
{
	m_MapSize.m_Height = y;
	m_MapSize.m_Width = x;
}