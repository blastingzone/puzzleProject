#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer(void)
{
}


CPlayer::~CPlayer(void)
{
}

void CPlayer::SetPlayerName( std::string playerName) //SM9: 스트링 복사 할거면 const std::string& 형태로 넘겨라.
{
	m_Name = playerName;
}

void CPlayer::SetPlayerTurn( int playerTurn )
{
	m_Turn = playerTurn;
}
