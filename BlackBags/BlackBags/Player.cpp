#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer(void)
{
	m_Id = 0;
	m_PlayerName = L"";
	m_PlayerImage = L"";
	m_PlayerBox = L"";

	//m_isMyTurn = 0;

	Init();
}


CPlayer::~CPlayer(void)
{
}

void CPlayer::Init()
{
	//m_isMyTurn = 0;
	m_isMyTurn = false;
	m_Turn = 0;

	m_MyTile = 0;
	m_MyGold = 0;
	m_MyTrash = 0;
}

int CPlayer::GetPlayerItemNumber(MO_ITEM itemType)
{
	switch(itemType)
	{
	case MO_NOTHING:
		return m_MyTile;
		break;
	case MO_GOLD:
		return m_MyGold;
		break;
	case MO_TRASH:
		return m_MyTrash;
		break;
	default:
		break;
	}
}

void CPlayer::UpdatePlayerResult(MO_ITEM item)
{
	++m_MyTile;

	switch (item)
	{
	case MO_GOLD:
		++m_MyGold;
		break;
	case MO_TRASH:
		++m_MyTrash;
		break;
	default:
		break;
	}
}