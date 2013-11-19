#pragma once
#include <iostream>

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	std::string		GetPlayerName(){return m_Name;}
	void			SetPlayerName(std::string playerName); //SM9: const string&
	void			SetPlayerTurn(int playerTurn);
	int				GetPlayerTurn(){return m_Turn;}
	void		    SetPlayerId(int playerId);
	int			    GetPlayerId(){return m_Id;}

private:
	int					m_Turn;
	std::string		m_Name;
	int				  m_Id;
};

