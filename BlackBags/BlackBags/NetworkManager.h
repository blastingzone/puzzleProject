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
	void ProcessPacket();

	void GetClientId();
	void Read();
	void Write();
	void CloseSocket();
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
};

