#pragma once

#include <string>

class CPlayer;

class CGameData
{
public:
	CGameData(void);
	~CGameData(void);

	static CGameData* CGameData::GetInstance();
	static void				ReleaseInstance();

	void Init();

	void		SetMapSize(int x, int y);
	MapSize		GetMapSize() {return m_MapSize;}

	void		SetPlayerNumber(int playerNumber)			{ m_PlayerNumber = playerNumber; }
	int			GetplayerNumber()							{ return m_PlayerNumber; }

	CPlayer*	GetPlayerPtr(int idx)						{ return m_PlayerData[idx]; }

	const std::wstring&		GetPlayerName(int idx);
	const std::wstring&		GetPlayerImage(int turn);
	const std::wstring&		GetPlayerBox(int turn);

	void		SetPlayerTurn(int idx, int playerTurn);
	int			GetPlayerTurn(int idx);

	void		UpdatePlayerResult(int id, MO_ITEM item);
	int			GetPlayerItemNumber(int id, MO_ITEM item);		

	void InitPlaySceneTimerFlag()							{ m_PlaySceneTimerFlag = false; }
	void SetPlaySceneTimerFlag()							{ m_PlaySceneTimerFlag = true; }
	bool GetPlaySceneTimerFlag()							{ return m_PlaySceneTimerFlag; }

	SceneName GetCurrentScene()								{ return m_CurrentScene; }
	void SetCurrentScene(const SceneName& currentScene)		{ m_CurrentScene = currentScene; }

	// Playermask (ex : 0001 = player1, 1001 = Player4, Player1)
	//void SetPlayerMask(int mask)							{ m_PlayerMask = mask; }
	//int GetPlayerMask()										{ return m_PlayerMask; }

private:
	void createPlayer();

	static CGameData*	m_pInstance; //singleton instance
	MapSize				m_MapSize;

	SceneName			m_CurrentScene;

	bool				m_PlaySceneTimerFlag;

	//int					m_PlayerMask;

	int					m_PlayerNumber;
	CPlayer*			m_PlayerData[MAX_PLAYER_NUM];
};
