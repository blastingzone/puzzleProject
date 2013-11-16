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

	int GetPlayerTileNumber(int playerId){return m_PlayerData[playerId].m_MyTile;}
	void SetPlayerTileNumber(int playerId, int tileNumber);

	int GetPlayerGoldNumber(int playerId){return m_PlayerData[playerId].m_MyGold;}
	void SetPlayerGoldNumber(int playerId, int goldNumber);

	int GetPlayerTrashNumber(int playerId){return m_PlayerData[playerId].m_MyTrash;}
	void SetPlayerTrashNumber(int playerId, int trashNumber);

	std::string GetPlayerName(int playerId){return m_PlayerData[playerId].m_PlayerName;}
private:
	static CGameData*		m_pInstance; //singleton instance
	
	int m_PlayerNumber;
	PlayerData m_PlayerData[MAX_PLAYER_NUM];

};

