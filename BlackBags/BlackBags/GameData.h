#pragma once

#include <string>
#include <d2d1.h>
#include <array>

class CPlayer;

class CGameData
{
public:
	CGameData(void);
	~CGameData(void);

	static CGameData* CGameData::GetInstance();
	static void				ReleaseInstance();

	void Init();
	void Release();

	void		SetMapSize(int x, int y);
	MapSize		GetMapSize() {return m_MapSize;}

	void		SetPlayerNumber(int playerNumber)			{ m_PlayerNumber = playerNumber; }
	int			GetplayerNumber()							{ return m_PlayerNumber; }

	CPlayer*	GetPlayerPtr(int idx)						{ return m_PlayerData[idx]; }

	const std::wstring&		GetPlayerName(int idx);
	//const std::wstring&		GetPlayerImage(int turn);
	//const std::wstring&		GetPlayerBox(int turn);
	//D2D1_COLOR_F	GetPlayerBrushColor(int turn);

	//ID2D1SolidColorBrush*	GetPlayerBrush(int turn);
	//ID2D1Bitmap*			GetPlayerFace(int turn);
	//ID2D1Bitmap*			GetPlayerBox(int turn);
	//ID2D1Bitmap*			GetPlayerWaitingBox(int turn);

	void		SetPlayerTurn(int idx, int playerTurn);
	int			GetPlayerTurn(int idx);

	//void		UpdatePlayerResult(int turn, MO_ITEM item);
	//int			GetPlayerItemNumber(int turn, MO_ITEM item);		

	void InitPlaySceneTimerFlag()	{ m_PlaySceneTimerFlag = false; }
	void SetPlaySceneTimerFlag()	{ m_PlaySceneTimerFlag = true; }
	bool GetPlaySceneTimerFlag()	{ return m_PlaySceneTimerFlag; }

	CPlayer*	GetPlayerPtrByTurn(int turn);

	SceneName GetCurrentScene()								{ return m_CurrentScene; }
	void SetCurrentScene(const SceneName& currentScene)		{ m_CurrentScene = currentScene; }

	//Playermask (ex : 0001 = player1, 1001 = Player4, Player1)
	void	SetPlayerMask(int mask);
	bool	GetPlayerCreatedFlag(int idx);

private:
	void createPlayer();

	static CGameData*	m_pInstance; //singleton instance
	MapSize				m_MapSize;

	SceneName			m_CurrentScene;

	bool				m_PlaySceneTimerFlag;

	int					m_PlayerMask;

	int					m_PlayerNumber;

	std::array<CPlayer*, MAX_PLAYER_NUM> m_PlayerData;
};
