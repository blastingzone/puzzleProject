﻿#pragma once

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

	void		SetPlayerTurn(int idx, int playerTurn);
	int			GetPlayerTurn(int idx);	

	void		InitPlaySceneTimerFlag()					{ m_PlaySceneTimerFlag = false; }
	void		SetPlaySceneTimerFlag()						{ m_PlaySceneTimerFlag = true; }
	bool		GetPlaySceneTimerFlag()						{ return m_PlaySceneTimerFlag; }

	CPlayer*	GetPlayerPtrByTurn(int turn);

	SceneName	GetCurrentScene()									{ return m_CurrentScene; }
	void		SetCurrentScene(const SceneName& currentScene)		{ m_CurrentScene = currentScene; }

	//Playermask (ex : 0001 = player1, 1001 = Player4, Player1)
	void		SetPlayerMask(int mask);
	bool		GetPlayerCreatedFlag(int idx);

	void		SetWindowsHwnd(HWND hWnd)						{ m_hWnd = hWnd; }
	HWND		GetWindowsHwnd()								{ if (nullptr != m_hWnd) return m_hWnd; }

	bool		GetNetworkNextSceneFlag()						{ return m_NetworkNextSceneFlag; }
	void		SetNetworkNextSceneFlag(bool flag)				{ m_NetworkNextSceneFlag = flag; }

	bool		GetNetworkRandomSeed()							{ return m_NetworkRandomSeed; }
	void		SetNetworkRandomSeed(unsigned int seed)			{ m_NetworkRandomSeed = seed; }

	int			GetCurrentTurnId()								{ return m_CurrentTurnId; }
	//조심해!! 예외처리를 잘 생각해서 불가능한 ID가 들어오지 않게!!
	void		SetCurrentTurnId(int id)						{ assert(id >= 0 && id < MAX_PLAYER_NUM ); m_CurrentTurnId = id; }

private:
	void		createPlayer();

	static CGameData*	m_pInstance; //singleton instance
	MapSize				m_MapSize;

	SceneName			m_CurrentScene;

	bool				m_PlaySceneTimerFlag;

	int					m_PlayerMask;

	int					m_PlayerNumber;

	std::array<CPlayer*, MAX_PLAYER_NUM> m_PlayerData;

	// 에러 헨들링을 위한 HWND
	// 아 이거 힘들게 만들었는데 안써도 되네요;; 메시지 박스 옵션이 알아서 함 ㅎㅎ
	HWND				m_hWnd;

	// 네트워크용 다음씬 넘어가는 조건 확인 플래그
	bool				m_NetworkNextSceneFlag;
	unsigned int		m_NetworkRandomSeed;

	// 현재 플레이어 턴을 저장하는 변수
	int					m_CurrentTurnId;
};
