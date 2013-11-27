#include "stdafx.h"
#include "GameData.h"

CGameData* CGameData::m_pInstance = nullptr;
CGameData::CGameData(void)
{
	//조심해!!
	//이름을 입력받던가 Player#1이렇게 바꿔.
	m_PlayerData[0].m_Id = 0;
	m_PlayerData[0].m_PlayerName = L"Jake Kim";

	m_PlayerData[1].m_Id = 1;
	m_PlayerData[1].m_PlayerName = L"Cassie Kim";

	m_PlayerData[2].m_Id = 2;
	m_PlayerData[2].m_PlayerName = L"Donald Kim";

	m_PlayerData[3].m_Id = 3;
	m_PlayerData[3].m_PlayerName = L"Lucy Kim";

	for (int i = 0;i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerData[i].m_MyTile = 0;
		m_PlayerData[i].m_MyGold = 0;
		m_PlayerData[i].m_MyTrash = 0;
	}

	SetMapSize(0, 0);

	m_CurrentScene = SC_NOSCENE;

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

//조심해!!
//하나로 합쳐야해~ 다른데도 바꿔줘야함
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

//조심해!! 이 3종도 합치도록한다. int? 앞에꺼랑 맞춰줘라.
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