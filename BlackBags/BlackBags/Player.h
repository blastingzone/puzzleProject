#pragma once
#include <iostream>

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	std::string		GetPlayerName(){return m_Name;}
	void			SetPlayerName(const std::string& playerName); //SM9: �̷������� const ���� Ȱ��. (��ü�� ���縦 ����)
	void			SetPlayerTurn(int playerTurn);
	int				GetPlayerTurn(){return m_Turn;}
	void		    SetPlayerId(int playerId);
	int			    GetPlayerId(){return m_Id;}

private:
	int					m_Turn;
	std::string		m_Name;
	int				  m_Id;
};

