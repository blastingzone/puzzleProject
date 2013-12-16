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

	// ����ó���� �θ��� �ʿ��� ( PlayerId < MAX_PLAYER_NUM )
	int	GetCharacterClientId(int characterId)		{ assert(characterId < MAX_PLAYER_NUM); return m_CharacterIdx[characterId]; }
	void InitCharaterList();

	// �� �δ콺 ����
	int GetMapIndex() { return m_MapIndex; }

	int GetPlayerNumber() { return m_PlayerNumber; }
	void SetPlayerNumber(int connectionNum) { m_PlayerNumber = connectionNum; }

private:
	static CNetworkManager*		m_pInstance; //singleton instance
	HWND						m_Hwnd;
	int							m_ClientId;

	char*						m_ServerAddr;
	int							m_Port;
	bool						m_LoginComplete;

	
	// ���� + ������ ���� + �ޱ� ����
	SOCKET						m_Socket;
	CircularBuffer				m_SendBuffer;
	CircularBuffer				m_RecvBuffer;

	// �� ������� ĳ���� ���� ������ �������� ���� �迭
	std::array<int, MAX_PLAYER_NUM> m_CharacterIdx;

	// ������ �� �δ콺
	int							m_MapIndex;

	// �÷��̾� ��
	int							m_PlayerNumber;
};

