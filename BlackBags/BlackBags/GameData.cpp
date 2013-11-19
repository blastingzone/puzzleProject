#include "stdafx.h"
#include "GameData.h"

CGameData* CGameData::m_pInstance = nullptr;
CGameData::CGameData(void)
{
	m_PlayerData[0].m_Id = 0;
	m_PlayerData[0].m_PlayerName = "Jake Kim"; //SM9: 이것들은 나중에 고칠거지? ㅋㅋ

	m_PlayerData[1].m_Id = 1;
	m_PlayerData[1].m_PlayerName = "Cassie Kim";

	m_PlayerData[2].m_Id = 2;
	m_PlayerData[2].m_PlayerName = "Donald Kim";

	m_PlayerData[3].m_Id = 3;
	m_PlayerData[3].m_PlayerName = "Lucy Kim";

	Init(); //SM9: 어차피 이 함수는 여기서밖에 안 쓰는데, 따로 분리한 이유가 있나?
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
	//SM9: 이거 이렇게 스위치 쓸 필요 없다

	//이렇게 한줄로 써도 된다. ++m_PlayerData[playerId].m_MyTile ;
	//그리고 MO_NOBODY가 playerId로 들어오는 일이 발생하지 않도록 프로그래밍 할 것
	//여기서는 단순히 assert(MO_NOBODY != playerId); 이렇게 어서트만 해주삼.

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
	//SM9: 아래도 마찬가지지... 
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
	//SM9: 이어동문
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