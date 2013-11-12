#pragma once
#include <iostream>

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	std::string		GetPlayerName(){return m_Name;}
	void			SetPlayerName(const std::string& playerName); //SM9: 이런식으로 const 적극 활용. (객체의 복사를 막음)
	void			SetPlayerTurn(int playerTurn);
	int				GetPlayerTurn(){return m_Turn;}
	void		    SetPlayerId(int playerId);
	int			    GetPlayerId(){return m_Id;}

private:
	int					m_Turn;
	std::string		m_Name;
	int				  m_Id;
};

