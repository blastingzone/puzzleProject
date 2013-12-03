#pragma once

#include <string>

struct PlayerData
{
	int m_Id;
	std::wstring m_PlayerName;
	std::wstring m_PlayerImage;
	std::wstring m_PlayerBox;

	bool m_isMyTurn;
	int m_Turn;

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

	void					Init();

	void		UpdatePlayerResult(int playerId, MO_ITEM item);

	int			GetPlayerTileNumber(int playerId);
	int			GetPlayerGoldNumber(int playerId);
	int			GetPlayerTrashNumber(int playerId);

	const std::wstring& GetPlayerName(int playerId)			{return m_PlayerData[playerId].m_PlayerName;} //SM9: 코드 정리좀 하삼...
	const std::wstring& GetPlayerImage(int playerId)			{return m_PlayerData[playerId].m_PlayerImage;} //SM9: 이렇게..
	const std::wstring& GetPlayerBox(int playerId)			{return m_PlayerData[playerId].m_PlayerBox;} //SM9: 이렇게..

	bool isPlayerTurn(int playerId) {return m_PlayerData[playerId].m_isMyTurn;}
	void isPlayerTurn(int playerId, bool isTurn) {m_PlayerData[playerId].m_isMyTurn = isTurn;}

	void SetPlayerTurn(int playerId, int turn) {m_PlayerData[playerId].m_Turn = turn;}
	int GetPlayerTurn(int palyerId) {return m_PlayerData[palyerId].m_Turn;}

	void SetPlayerIdAndName(int playerId,const std::wstring& playerName) {m_PlayerData[playerId].m_PlayerName = playerName;}

	void		SetMapSize(int x, int y);
	MapSize		GetMapSize() {return m_MapSize;}

	void		SetPlayerNumber(int playerNumber)			{m_PlayerNumber = playerNumber;}
	int			GetplayerNumber()							{return m_PlayerNumber; }

	void InitPlaySceneTimerFlag()							{ m_PlaySceneTimerFlag = false; }
	void SetPlaySceneTimerFlag()							{ m_PlaySceneTimerFlag = true; }
	bool GetPlaySceneTimerFlag()							{ return m_PlaySceneTimerFlag; }

	SceneName GetCurrentScene()								{ return m_CurrentScene; }
	void SetCurrentScene(const SceneName& currentScene)		{ m_CurrentScene = currentScene; }

	// Playermask (ex : 0001 = player1, 1001 = Player4, Player1)
	void SetPlayerMask(int mask)							{ m_PlayerMask = mask; }
	int GetPlayerMask()										{ return m_PlayerMask; }

private:
	static CGameData*		m_pInstance; //singleton instance
	MapSize					m_MapSize;

	SceneName				m_CurrentScene;

	bool					m_PlaySceneTimerFlag;

	int						m_PlayerMask;

	int						m_PlayerNumber;
	PlayerData				m_PlayerData[MAX_PLAYER_NUM];
};
