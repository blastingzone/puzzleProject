﻿#pragma once

#include <winsock2.h>
#include "CircularBuffer.h"

class CNetworkManager
{
public:
	CNetworkManager(void);
	~CNetworkManager(void);

	static CNetworkManager*		CNetworkManager::GetInstance();
	static void					ReleaseInstance();

	//조심해!
	//init과 생성자에서 게임에 관련된 변수들은 따로 빼서 초기화 할 수 있도록 할 것
	//게임이 종료되고 나면 다시 연결을 시도하면서 connect 실패함 -> 변수 초기화하면서 실패하지 않도록 만들 것
	bool	Init();
	void	SetHwnd(HWND hwnd);

	bool Connect();
	void ProcessPacket();

	int GetClientId()					{ return m_ClientId; }

	CircularBuffer*	GetSendBuffer()		{ return &m_SendBuffer; }
	CircularBuffer*	GetRecvBuffer()		{ return &m_RecvBuffer; }

	void PostSendMessage();

	void AskClientId();
	void Read();
	void Write();
	void CloseSocket();

	// 예외처리는 부르는 쪽에서 ( PlayerId < MAX_PLAYER_NUM )
	int	GetCharacterClientId(int characterId)					{ assert(characterId < MAX_PLAYER_NUM); return m_CharacterIdx[characterId]; }
	void InitCharaterList();

	// 맵 인댁스 전달
	int GetMapIndex()											{ return m_MapIndex; }

	int GetPlayerNumber()										{ return m_PlayerNumber; }
	void SetPlayerNumber(int connectionNum)						{ m_PlayerNumber = connectionNum; }

	int			GetCurrentTurnId()								{ return m_CurrentTurnId; }
	void		SetCurrentTurnId(int id)						{ assert(id >= 0 && id < MAX_PLAYER_NUM ); m_CurrentTurnId = id; }


	bool			GetDrawLineFlag()							{ return m_DrawLineFlag; }
	void			SetDrawLineFlag(bool flag)					{ m_DrawLineFlag = flag; }
	IndexedPosition GetIndexedPositionFromServer()				{ return m_IndexedPositionFromServer; }

private:
	static CNetworkManager*		m_pInstance; //singleton instance
	HWND						m_Hwnd;
	// 내 아이디. 중요하다.
	int							m_ClientId;

	char*						m_ServerAddr;
	int							m_Port;

	
	// 소켓 + 보내기 버퍼 + 받기 버퍼
	SOCKET						m_Socket;
	CircularBuffer				m_SendBuffer;
	CircularBuffer				m_RecvBuffer;

	// 각 사용자의 캐릭터 선택 정보를 가져오기 위한 배열
	// 인덱스 : 캐릭터 , 값 : 플레이어 ID
	std::array<int, MAX_PLAYER_NUM> m_CharacterIdx;

	// 선택한 맵 인댁스
	int							m_MapIndex;

	// 플레이어 수
	int							m_PlayerNumber;

	// 현재 플레이어 턴을 저장하는 변수
	int							m_CurrentTurnId;

	// 서버한테서 받아온 인덱스를 저장해두고 나중에 그릴 수 있게 해 줌
	bool						m_DrawLineFlag;
	IndexedPosition				m_IndexedPositionFromServer;
};

