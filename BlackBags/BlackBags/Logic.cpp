#include "stdafx.h"
#include "Logic.h"
#include "MacroSet.h"

CLogic* CLogic::m_pInstance = nullptr;
CLogic::CLogic(void)
{
}


CLogic::~CLogic(void)
{
}

CLogic* CLogic::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLogic();
	}

	return m_pInstance;
}

bool CLogic::Release()
{
	for ( int i = 0; i< m_PlayerNumber ; ++i)
	{

		if(m_Player[i])
		{
			delete m_Player[i];
			m_Player[i] = NULL;
		}
	}

	return true;
}


bool CLogic::ReleaseInstance()
{
	m_pInstance->Release();
	delete m_pInstance;

	return true;
}


//Logic���� �ʱ�ȭ �Լ�
void CLogic::Init()
{
	memset(m_Player,0,sizeof(m_Player));
	GetPlayerNumber();
	CreatePlayers();
	SetPlayerTurn();
}

//���� ���� ������ ������Ʈ ���ִ� �Լ�
void CLogic::Update( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	indexedPosition = CalcualteIndex(mouseCoordinate);

	//IsPossible üũ �Ŀ� gameMap ȣ���ؼ� �ݿ�
	m_Map->GetInstance()->DrawLine(indexedPosition);
}

//���콺 ��ǥ���� index�� �ٲٴ� �Լ�
IndexedPosition CLogic::CalcualteIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;

	//���콺�� ��ġ�� ���� �׷����� ������ ��ǥ�踦 �������� ��ȯ
	mouseCoordinate.m_PosX -= (int)(m_Map->GetInstance()->GetStartPosition().width);
	mouseCoordinate.m_PosY -= (int)(m_Map->GetInstance()->GetStartPosition().height);

	//Ÿ�� �ϳ��� ���� �ϳ��� ��� ��ⷯ �������� �ε��� �� ���
	indexedPosition.m_PosI = 
		( mouseCoordinate.m_PosY / (int) (TILE_SIZE + LINE_WEIGHT) ) * 2 
		+ ( ( mouseCoordinate.m_PosY % (int) (TILE_SIZE + LINE_WEIGHT) > LINE_WEIGHT ) ? 2 : 1);
	indexedPosition.m_PosJ = 
		( mouseCoordinate.m_PosX / (int) (TILE_SIZE + LINE_WEIGHT) ) * 2 
		+ ( ( mouseCoordinate.m_PosX % (int) (TILE_SIZE + LINE_WEIGHT) > LINE_WEIGHT ) ? 2 : 1);
	
	return indexedPosition;
}

bool CLogic::GetPlayerNumber()
{
	//����ȭ�鿡�� �÷��̾� ���� ����!
	m_PlayerNumber = 3;

	return true;
}

bool CLogic::CreatePlayers()
{
	for (int i = 0; i<m_PlayerNumber;++i)
	{
		m_Player[i] = new CPlayer();

		m_Player[i]->SetPlayerName("hihihi");
		m_Player[i]->SetPlayerTurn(-1);
	}

	return true;
}

bool CLogic::SetPlayerTurn()
{
	CPlayer * tempPlayer[4];
	memcpy(tempPlayer,m_Player,sizeof(m_Player));
	
	bool flag;
	int i = 0;
	while (i < m_PlayerNumber)
	{
		int tempTurn = rand()%m_PlayerNumber;
		flag = true;
		m_Player[i]->SetPlayerTurn(tempTurn);

		for (int j = 0; j<i ; ++j)
		{
			if (m_Player[i]->GetPlayerTurn() == m_Player[j]->GetPlayerTurn())
			{
				flag = false;
			}
		}

		if (flag == false)
			continue;

		tempPlayer[tempTurn] = m_Player[i];
		++i;
	}
	memcpy(m_Player,tempPlayer,sizeof(m_Player));

	return true;
}
