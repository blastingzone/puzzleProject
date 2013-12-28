#pragma once
#include <iostream>
#include <d2d1.h>

class CPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	bool Init(MO_OWNER playerIdx);
	void ResetValues();

	void					SetPlayerName(const std::wstring& playerName)	{ m_PlayerName = playerName; }
	const std::wstring&		GetPlayerName()									{ return m_PlayerName; }
	
	ID2D1SolidColorBrush*	GetPlayerBrush()		{ return m_pTileColorBrush; }
	ID2D1Bitmap*			GetPlayerFace()			{ return m_pPlayerFace; }

	void	SetPlayerTurn(int playerTurn)			{ m_Turn = playerTurn; }
	int		GetPlayerTurn()							{ return m_Turn; }

	void	SetPlayerId(int playerId)				{ m_Id = playerId; }
	int		GetPlayerId()							{ return m_Id; }

	int		GetPlayerItemNumber(MO_ITEM itemType);


	void	UpdatePlayerResult(MO_ITEM item);

	void	SetPlayerCreatedFlag()	{ m_isJoinGame = true; }
	bool	GetPlayerCreatedFlag()	{ return m_isJoinGame; }

private:
	int m_Id;

	std::wstring m_PlayerName;

	bool m_isJoinGame;
	bool m_Created;
	int m_Turn;

	int m_MyTile;
	int m_MyGold;
	int m_MyTrash;

	ID2D1SolidColorBrush*	m_pTileColorBrush;
	ID2D1Bitmap*			m_pPlayerFace;
};

