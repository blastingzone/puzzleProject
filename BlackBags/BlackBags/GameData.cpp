#include "stdafx.h"
#include "GameData.h"

CGameData* CGameData::m_pInstance = nullptr;

CGameData::CGameData(void)
{
	m_CurrentScene = SC_NOSCENE;

	Init();

	m_PlayerMask = 0;
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

void CGameData::Init()
{
	//생성자와 중복이 너무 많음
	//타일, 골드, 라인 수만 초기화 해주는 함수 만드는 것이 좋을까요?


	for (int i = 0;i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerData[i].m_PlayerId = i;
		m_PlayerData[i].m_MyTile = 0;
		m_PlayerData[i].m_MyGold = 0;
		m_PlayerData[i].m_MyTrash = 0;

		switch(i)
		{
		case 0:
			m_PlayerData[0].m_PlayerName = L"Player1";
			m_Character[0].m_PortraitPath = L"Resource/Image/player1.png";
			m_Character[0].m_BoxPath = L"Resource/Image/player1Box.png";
			m_Character[0].m_BrushColor = D2D1::ColorF(_COLOR_CHARACTER_1_);
			break;
		case 1:
			m_PlayerData[1].m_PlayerName = L"Player2";
			m_Character[1].m_PortraitPath = L"Resource/Image/player2.png";
			m_Character[1].m_BoxPath = L"Resource/Image/player2Box.png";
			m_Character[1].m_BrushColor = D2D1::ColorF(_COLOR_CHARACTER_2_);
			break;
		case 2:
			m_PlayerData[2].m_PlayerName = L"Player3";
			m_Character[2].m_PortraitPath = L"Resource/Image/player3.png";
			m_Character[2].m_BoxPath = L"Resource/Image/player3Box.png";
			m_Character[2].m_BrushColor = D2D1::ColorF(_COLOR_CHARACTER_3_);
			break;
		case 3:
			m_PlayerData[3].m_PlayerName = L"Player4";
			m_Character[3].m_PortraitPath = L"Resource/Image/player4.png";
			m_Character[3].m_BoxPath = L"Resource/Image/player4Box.png";
			m_Character[3].m_BrushColor = D2D1::ColorF(_COLOR_CHARACTER_4_);
			break;
			
		}
	}

	SetMapSize(0, 0);

	m_PlaySceneTimerFlag = false;

}

void CGameData::UpdatePlayerResult( int playerId, MO_ITEM item )
{
	assert( (0 <= playerId) && (playerId < MAX_PLAYER_NUM) );
	++m_PlayerData[playerId].m_MyTile;
	switch (item)
	{
	case MO_GOLD:
		++m_PlayerData[playerId].m_MyGold;
		break;
	case MO_TRASH:
		++m_PlayerData[playerId].m_MyTrash;
		break;
	default:
		break;
	}
}


void CGameData::SetMapSize(int x, int y)
{
	m_MapSize.m_Height = y;
	m_MapSize.m_Width = x;
}

int CGameData::GetPlayerItemNumber(int playerId, MO_ITEM itemType)
{
	assert(MO_NOBODY!=playerId);

	switch(itemType)
	{
	case MO_NOTHING:
		return m_PlayerData[playerId].m_MyTile;
		break;
	case MO_GOLD:
		return m_PlayerData[playerId].m_MyGold;
		break;
	case MO_TRASH:
		return m_PlayerData[playerId].m_MyTrash;
		break;
	default:
		break;
	}
}