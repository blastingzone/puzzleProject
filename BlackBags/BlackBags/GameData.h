#pragma once

#include <string>
#include <d2d1.h>

struct PlayerData
{
	int m_PlayerId;

	std::wstring m_PlayerName;

	//내가 가지고 있는 캐릭터 아이디 -> 캐릭터의 색상, 이미지, 타일 색상은 여기서 결정된다.
	int m_CharacterId;

	int m_PlayTurn;

	int m_MyTile;
	int m_MyGold;
	int m_MyTrash;

};

//Charater는 캐릭터 + 관련된 색상에 관한 값이다. 
//Player가 다른 Character로 바꿀경우, CharacterNumber만 바꿔주면 된다.
struct Character
{
	std::wstring m_PortraitPath;
	std::wstring m_BoxPath;
	D2D1_COLOR_F m_BrushColor;
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

	int GetPlayerItemNumber(int playerId, MO_ITEM itemType);

	void SetPlayerCharacterId(int playerId, int characterId) { m_PlayerData[playerId].m_CharacterId = characterId;}

	const std::wstring& GetPlayerName(int playerId)			{ return m_PlayerData[playerId].m_PlayerName; }
	const std::wstring& GetPlayerImage(int playerId)		{ return m_Character[m_PlayerData[playerId].m_CharacterId].m_PortraitPath; }
	const std::wstring& GetPlayerBox(int playerId)			{ return m_Character[m_PlayerData[playerId].m_CharacterId].m_BoxPath; } 
	const D2D1_COLOR_F& GetPlayerColor(int playerId) { return m_Character[m_PlayerData[playerId].m_CharacterId].m_BrushColor; }

	void SetPlayerTurn(int playerId, int turn)				{ m_PlayerData[playerId].m_PlayTurn = turn; }
	int GetPlayerTurn(int palyerId)							{ return m_PlayerData[palyerId].m_PlayTurn; }

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
	Character			m_Character[MAX_PLAYER_NUM];
};
