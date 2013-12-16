#pragma once

#include <winsock2.h>
#include "CircularBuffer.h"

class CNetworkManager
{
public:
	CNetworkManager(void);
	~CNetworkManager(void);

	static CNetworkManager*		CNetworkManager::GetInstance();
	static void					ReleaseInstance();

	bool	Init(HWND hwnd);

	bool Connect();
	bool IsLoginComplete()				{ return m_LoginComplete; }
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
	int	GetCharacterClientId(int characterId)		{ assert(characterId < MAX_PLAYER_NUM); return m_CharacterIdx[characterId]; }
	void InitCharaterList();

	// 맵 인댁스 전달
	int GetMapIndex() { return m_MapIndex; }

private:
	static CNetworkManager*		m_pInstance; //singleton instance
	HWND						m_Hwnd;
	int							m_ClientId;

	char*						m_ServerAddr;
	int							m_Port;
	bool						m_LoginComplete;

	
	// 소켓 + 보내기 버퍼 + 받기 버퍼
	SOCKET						m_Socket;
	CircularBuffer				m_SendBuffer;
	CircularBuffer				m_RecvBuffer;

	// 각 사용자의 캐릭터 선택 정보를 가져오기 위한 배열
	int							m_CharacterIdx[MAX_PLAYER_NUM];

	// 선택한 맵 인댁스
	int							m_MapIndex;
};

