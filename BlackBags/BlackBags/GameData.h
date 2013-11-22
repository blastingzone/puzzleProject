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


	int			GetPlayerTileNumber(int playerId);
	void		IncreasePlayerTileNumber(MO_OWNER playerId);

	int			GetPlayerGoldNumber(int playerId);
	void		IncreasePlayerGoldNumber(int playerId);

	int			GetPlayerTrashNumber(int playerId);
	void		IncreasePlayerTrashNumber(int playerId);

	const std::string GetPlayerName(int playerId)			{return m_PlayerData[playerId].m_PlayerName;}

	void		SetMapSize(int x, int y);
	MapSize		GetMapSize() {return m_MapSize;}

	void		SetPlayerNumber(int playerNumber)	{m_PlayerNumber = playerNumber;}
	int			GetplayerNumber()					{return m_PlayerNumber; }

	void InitPlaySceneTimerFlag() { m_PlaySceneTimerFlag = false; }
	void SetPlaySceneTimerFlag() { m_PlaySceneTimerFlag = true; }
	bool GetPlaySceneTimerFlag() { return m_PlaySceneTimerFlag; }

private:
	static CGameData*		m_pInstance; //singleton instance
	MapSize m_MapSize;

	bool m_PlaySceneTimerFlag;

	int m_PlayerNumber;
	PlayerData m_PlayerData[MAX_PLAYER_NUM];
};
