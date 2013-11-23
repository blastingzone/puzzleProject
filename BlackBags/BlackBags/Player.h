#pragma once
#include <iostream>

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	std::wstring		GetPlayerName() {return m_Name;}
	void				SetPlayerName(const std::wstring& playerName);
	void				SetPlayerTurn(int playerTurn);
	int					GetPlayerTurn() {return m_Turn;}
	void				SetPlayerId(int playerId);
	int					GetPlayerId() {return m_Id;}

private:
	int					m_Turn;
	std::wstring		m_Name;
	int					m_Id;
};

