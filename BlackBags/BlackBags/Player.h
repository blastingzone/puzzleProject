#pragma once
#include <iostream>
#include <d2d1.h>

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
	
	void			SetBrushColor(D2D1_COLOR_F color)	{ m_BrushColor = color; }
	D2D1_COLOR_F	GetBrushColor()						{ return m_BrushColor; }

	void	SetPlayerTurn(int playerTurn)			{ m_Turn = playerTurn; }
	int		GetPlayerTurn()							{ return m_Turn; }

	void	SetPlayerId(int playerId)				{ m_Id = playerId; }
	int		GetPlayerId()							{ return m_Id; }

	int		GetPlayerItemNumber(MO_ITEM itemType);

	void	SetPlayerTurn(int playerId, int turn)	{ m_Turn = turn; }
	int		GetPlayerTurn(int palyerId)				{ return m_Turn; }

	void	UpdatePlayerResult(MO_ITEM item);

	void	SetPlayerCreatedFlag()	{ m_isMyTurn = true; }
	bool	GetPlayerCreatedFlag()	{ return m_isMyTurn; }

private:
	int m_Id;

	std::wstring m_PlayerName;
	std::wstring m_PlayerImage;
	std::wstring m_PlayerBox;

	bool m_isMyTurn;
	bool m_Created;
	int m_Turn;

	int m_MyTile;
	int m_MyGold;
	int m_MyTrash;

	D2D1_COLOR_F m_BrushColor;
};

