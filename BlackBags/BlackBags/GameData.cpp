#include "stdafx.h"
#include "GameData.h"

CGameData* CGameData::m_pInstance = nullptr;

CGameData::CGameData(void)
{
	// ������!! �� �κ� Init()���� �ִµ� ���� ���°� �´� ���ϱ��?
	for (int i = 0;i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerData[i].m_Id = i;
		m_PlayerData[i].m_MyTile = 0;
		m_PlayerData[i].m_MyGold = 0;
		m_PlayerData[i].m_MyTrash = 0;
		m_PlayerData[i].m_isMyTurn = false;
	}

	SetMapSize(0, 0);

	m_CurrentScene = SC_NOSCENE;

	m_PlaySceneTimerFlag = false;

	m_PlayerMask = 0;
}


CGameData::~CGameData(void)
{
}

CGameData* CGameData::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameData();
	}

	return m_pInstance;
}

void CGameData::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}

void CGameData::Init()
{
	//�����ڿ� �ߺ��� �ʹ� ����
	//Ÿ��, ���, ���� ���� �ʱ�ȭ ���ִ� �Լ� ����� ���� �������?
	m_PlayerData[0].m_PlayerName = L"Player1";
	m_PlayerData[0].m_PlayerImage = L"Resource/Image/player1.png";
	m_PlayerData[0].m_PlayerBox = L"Resource/Image/player1Box.png";

	m_PlayerData[1].m_PlayerName = L"Player2";
	m_PlayerData[1].m_PlayerImage = L"Resource/Image/player2.png";
	m_PlayerData[1].m_PlayerBox = L"Resource/Image/player2Box.png";

	m_PlayerData[2].m_PlayerName = L"Player3";
	m_PlayerData[2].m_PlayerImage = L"Resource/Image/player3.png";
	m_PlayerData[2].m_PlayerBox = L"Resource/Image/player3Box.png";

	m_PlayerData[3].m_PlayerName = L"Player4";
	m_PlayerData[3].m_PlayerImage = L"Resource/Image/player4.png";
	m_PlayerData[3].m_PlayerBox = L"Resource/Image/player4Box.png";

	for (int i = 0;i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerData[i].m_MyTile = 0;
		m_PlayerData[i].m_MyGold = 0;
		m_PlayerData[i].m_MyTrash = 0;
	}

	SetMapSize(0, 0);

	m_PlaySceneTimerFlag = false;

}

void CGameData::UpdatePlayerResult( int playerId, MO_ITEM item )
{
	assert( (0 <= playerId) && (playerId < MAX_PLAYER_NUM) );
	++m_PlayerData[playerId].m_MyTile;
	switch (item)
	{
	case MO_GOLD:
		++m_PlayerData[playerId].m_MyGold;
		break;
	case MO_TRASH:
		++m_PlayerData[playerId].m_MyTrash;
		break;
	default:
		break;
	}
}


void CGameData::SetMapSize(int x, int y)
{
	m_MapSize.m_Height = y;
	m_MapSize.m_Width = x;
}

// �� 3���� ��ġ�����Ѵ�. int? �տ����� �������.
// �̰�...... ������ �Լ����� ���Ǵ� ��찡 ���ε��ζ� ��ġ�Ⱑ ��ƽ��ϴ�
int CGameData::GetPlayerTileNumber( int playerId )
{
	assert(MO_NOBODY!=playerId);
	return m_PlayerData[playerId].m_MyTile;
}

int CGameData::GetPlayerGoldNumber( int playerId )
{
	assert(MO_NOBODY!=playerId);
	return m_PlayerData[playerId].m_MyGold;
}
int CGameData::GetPlayerTrashNumber( int playerId )
{
	assert(MO_NOBODY!=playerId);
	return m_PlayerData[playerId].m_MyTrash;
}