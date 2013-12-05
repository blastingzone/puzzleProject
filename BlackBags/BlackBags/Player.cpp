#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer(void)
{
	m_Turn = -1;
	m_Name = L"";
	m_Id = -1;
}


CPlayer::~CPlayer(void)
{
}

void CPlayer::SetPlayerName( const std::wstring& playerName)
{
	m_Name = playerName;
}

void CPlayer::SetPlayerTurn( int playerTurn )
{
	m_Turn = playerTurn;
}

void CPlayer::SetPlayerId(int playerId)
{
	m_Id = playerId;
}