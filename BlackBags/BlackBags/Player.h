#pragma once
#include <iostream>

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	void Init();

	void					SetPlayerName(const std::wstring& playerName)	{ m_PlayerName = playerName; }
	const std::wstring&		GetPlayerName()									{ return m_PlayerName; }

	void					SetPlayerImage(const std::wstring& playerImage)	{ m_PlayerImage = playerImage; }
	const std::wstring&		GetPlayerImage()								{ return m_PlayerImage; }
	
	void					SetPlayerBox(const std::wstring& playeBox)		{ m_PlayerBox = playeBox; }
	const std::wstring&		GetPlayerBox()									{ return m_PlayerBox; } 
	
	void	SetPlayerTurn(int playerTurn)			{ m_Turn = playerTurn; }
	int		GetPlayerTurn()							{ return m_Turn; }

	void	SetPlayerId(int playerId)				{ m_Id = playerId; }
	int		GetPlayerId()							{ return m_Id; }

	int		GetPlayerItemNumber(MO_ITEM itemType);

	void	SetPlayerTurn(int playerId, int turn)	{ m_Turn = turn; }
	int		GetPlayerTurn(int palyerId)				{ return m_Turn; }

	void	SetPlayerCreatedFlag()					{ m_Created = true; }
	int		GetPlayerCreatedFlag()					{ return m_Created; }

	void	UpdatePlayerResult(MO_ITEM item);

private:
	int m_Id;

	std::wstring m_PlayerName;
	std::wstring m_PlayerImage;
	std::wstring m_PlayerBox;

	//bool m_isMyTurn;
	bool m_Created;
	int m_Turn;

	int m_MyTile;
	int m_MyGold;
	int m_MyTrash;
};

