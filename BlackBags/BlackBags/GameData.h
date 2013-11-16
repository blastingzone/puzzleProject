#pragma once
#include <string>

struct PlayerData
{
	int m_Id;
	std::string m_PlayerName;

	int m_MyTile;
	int m_MyGold;
	int m_MyTrash;

};
class CGameData
{
public:
	CGameData(void);
	~CGameData(void);

	static CGameData* CGameData::GetInstance();
	static void				ReleaseInstance();

	void Init();

	int			GetPlayerTileNumber(int playerId)	{return m_PlayerData[playerId].m_MyTile;}
	void		IncreasePlayerTileNumber(MO_OWNER playerId);

	int			GetPlayerGoldNumber(int playerId)	{return m_PlayerData[playerId].m_MyGold;}
	void		IncreasePlayerGoldNumber(int playerId);

	int			GetPlayerTrashNumber(int playerId)	{return m_PlayerData[playerId].m_MyTrash;}
	void		IncreasePlayerTrashNumber(int playerId);

	std::string GetPlayerName(int playerId)			{return m_PlayerData[playerId].m_PlayerName;}

	void		SetMapSize(int x, int y);
	MapSize		GetMapSize() {return m_MapSize;}

	void		SetPlayerNumber(int playerNumber)	{m_PlayerNumber = playerNumber;}
	int			GetplayerNumber()					{return m_PlayerNumber; }

private:
	static CGameData*		m_pInstance; //singleton instance
	
	MapSize m_MapSize;

	int m_PlayerNumber;
	PlayerData m_PlayerData[MAX_PLAYER_NUM];

};
