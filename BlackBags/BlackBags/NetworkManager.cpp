#include "stdafx.h"
#include "NetworkManager.h"
#include "PacketType.h"

#pragma comment(lib,"ws2_32.lib")

#define WM_SOCKET		104

#define BUFSIZE	1024*10

CNetworkManager* CNetworkManager::m_pInstance = nullptr;

CNetworkManager::CNetworkManager(void) : m_SendBuffer(BUFSIZE), m_RecvBuffer(BUFSIZE)
{
	m_Hwnd = NULL;
	m_ClientId = -1;

	m_ServerAddr = "localhost" ;
	m_Port = 9001 ;
	m_LoginComplete = false ;
	m_Socket = NULL ;

	m_MapIndex = -1;
	InitCharaterList();
}

CNetworkManager::~CNetworkManager(void)
{
}

CNetworkManager* CNetworkManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CNetworkManager();
	}

	return m_pInstance;
}

bool CNetworkManager::Init(HWND hwnd)
{
	m_Hwnd = hwnd;

	WSADATA WsaDat ;

	int nResult = WSAStartup(MAKEWORD(2,2),&WsaDat) ;
	if ( nResult != 0 )
		return false ;

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) ;
	if ( m_Socket == INVALID_SOCKET )
		return false ;

	return true ;
}

void CNetworkManager::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}

bool CNetworkManager::Connect()
{
	// Resolve IP address for hostname
	struct hostent* host ;

	if ( (host=gethostbyname(m_ServerAddr) ) == NULL )
		return false ;

	// Set up our socket address structure
	SOCKADDR_IN SockAddr ;
	SockAddr.sin_port = htons(m_Port) ;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr) ;

	if ( SOCKET_ERROR == connect(m_Socket, (LPSOCKADDR)(&SockAddr), sizeof(SockAddr)) )
	{
		if (GetLastError() != WSAEWOULDBLOCK )
			return false ;
	}
	

	return true ;
}

/// ��Ŷó�� 
void CNetworkManager::ProcessPacket()
{
	while ( true )
	{
		PacketHeader header ;

		if ( false == m_RecvBuffer.Peek((char*)&header, sizeof(PacketHeader)) )
			break ;

		if (header.mSize > m_RecvBuffer.GetCurrentSize() )
			break ;

		switch ( header.mType )
		{
		case PKT_SC_LOGIN:
			{
				LoginResult recvData ;
				if ( m_RecvBuffer.Read((char*)&recvData, header.mSize) )
				{
					// ��Ŷó��
					if ( recvData.mPlayerId == -1  )
					{
						/// ���� �ɸ��� �α��� ���д�.
						ExitProcess(-1) ;
					}
					
					m_ClientId = recvData.mPlayerId;
					m_LoginComplete = true ;
					m_PlayerNumber = recvData.mPlayerNumber;
				
				}
				else
				{
					assert(false) ;
				}
			}
			break ;

		case PKT_SC_CHARACTER_SELECT:
			{
				CharacterResult recvData;
				if ( m_RecvBuffer.Read((char*)&recvData, header.mSize) )
				{
					InitCharaterList();

					for (int PlayerIdx = 0; PlayerIdx < MAX_PLAYER_NUM ; ++PlayerIdx)
					{
						//���ʴ�� Ȯ���ϸ鼭 PlayerIdx�� �ش��ϴ� Ŭ���̾�Ʈ�� ������ ĳ���Ͱ� ������
						//�� ĳ���� �迭�� ������ PlayerIdx�� ����Ѵ�.

						//������!!!!
						//��Ŷ������ �ε����� Ŭ���̾�Ʈ ���̵�(PlayerIdx)�� ����ϰ� �����͸� ĳ���� �ѹ��� ���
						//�ݸ鿡 Ŭ���̾�Ʈ������ ĳ���� �ѹ��� �ε����� ����ϰ� �����ͷ� Ŭ���̾�Ʈ ���̵� ����Ѵ�
						//���ϰ� �̻��ϴ�. ���� �ڵ�� ȣ�ϼ��� �����ؼ� ��Ŷ ������ ������ �ʿ��ϴ�
						if (recvData.mCharacterId[PlayerIdx] != -1)
						{
							m_CharacterIdx[recvData.mCharacterId[PlayerIdx] ] = PlayerIdx;
						}
					}
				}
			}
			break;

		case PKT_SC_MAP_SELECT:
			{
				MapResult recvData;
				if (m_RecvBuffer.Read((char*)&recvData, header.mSize))
				{
					m_MapIndex = recvData.mMapIdx;
				}
			}
			break;

		case PKT_SC_IDX:
			{
				EventPositionResult recvData ;
				if ( m_RecvBuffer.Read((char*)&recvData, header.mSize) )
				{
					char buff[MAX_CHAT_LEN] = {0, } ;
					sprintf_s(buff, "CHAT from Player[%s]: X : %d, Y : %d \n", recvData.mName, recvData.m_Xpos, recvData.m_Ypos ) ;

					static int y2pos = 60 ;
					HDC hdc = GetDC(m_Hwnd) ;
					TextOutA(hdc, 10, y2pos, buff, strlen(buff)) ;
					ReleaseDC(m_Hwnd, hdc) ;
					y2pos += 15 ;
					if ( y2pos > 600 )
						y2pos = 60 ;

				}
				else
				{
					assert(false) ;
				}
			}
			break ;

		default:
			assert(false) ;
		}

	}
}

void CNetworkManager::AskClientId()
{
	/// NAGLE ����
	int opt = 1 ;
	::setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)) ;

	LoginRequest sendData ;
	sendData.mPlayerId = -1;

	if ( m_SendBuffer.Write((const char*)&sendData, sendData.mSize) )
	{
		PostMessage(m_Hwnd, WM_SOCKET, 0, FD_WRITE) ;
	}


	int nResult = WSAAsyncSelect(m_Socket, m_Hwnd, WM_SOCKET, (FD_CLOSE|FD_READ|FD_WRITE) ) ;
	if (nResult)
	{
		assert(false) ;
	}
}


void CNetworkManager::Read()
{
	char inBuf[4096] = {0, } ;
					
	int recvLen = recv(m_Socket, inBuf, 4096, 0) ;

	if ( !m_RecvBuffer.Write(inBuf, recvLen) )
	{
		/// ���� ��á��. 
		assert(false) ;
	}
					
	ProcessPacket() ;
}

void CNetworkManager::Write()
{
	/// ������ ���ۿ� �ִ°͵� ������ ������
	int size = m_SendBuffer.GetCurrentSize() ;
	if ( size > 0 )
	{
		char* data = new char[size] ;
		m_SendBuffer.Peek(data) ;

		int sent = send(m_Socket, data, size, 0) ;
						
		/// �ٸ��� �ִ�
		if ( sent != size )
			OutputDebugStringA("sent != request\n") ;

		m_SendBuffer.Consume(sent) ;

		delete [] data ;
	}
}

void CNetworkManager::CloseSocket()
{
	MessageBox(m_Hwnd, L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION|MB_OK);
	closesocket(m_Socket);

	//Scene change - > to mainScene
}

void CNetworkManager::PostSendMessage()
{
	PostMessage(m_Hwnd, WM_SOCKET, 0, FD_WRITE);
}

void CNetworkManager::InitCharaterList()
{
	for (int i = 0; i < MAX_PLAYER_NUM ; ++i)
	{
		m_CharacterIdx[i] = -1;
	}
}