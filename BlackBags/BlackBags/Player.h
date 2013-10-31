#pragma once
#include <iostream>

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);


	std::string GetPlayerName(){return m_Name;}
	void SetPlayerName(std::string playerName);
	void SetPlayerTurn(int playerTurn);
	int GetPlayerTurn(){return m_Turn;}

private:
	int					m_Turn;
	std::string		m_Name;


};

