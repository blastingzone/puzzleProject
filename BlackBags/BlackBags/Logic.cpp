#include "stdafx.h"
#include "Logic.h"
#include "MacroSet.h"
#include <queue>

CLogic* CLogic::m_pInstance = nullptr;
CLogic::CLogic(void)
{
	AllocConsole();
	FILE* pFile;
	freopen_s(&pFile, "CONOUT$", "wb", stdout);
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

		if (m_Player[i])
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


//Logic관련 초기화 함수
void CLogic::Init()
{
	m_Map = m_Map->GetInstance();
	memset(m_Player,0,sizeof(m_Player));
	GetPlayerNumber();
	CreatePlayers();
	SetPlayerTurn();
}

//지도 관련 정보를 업데이트 해주는 함수
void CLogic::Update( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	indexedPosition = CalcualteIndex(mouseCoordinate);

	printf(" i : %d, j : %d\n",indexedPosition.m_PosI,indexedPosition.m_PosJ);
	
	//IsPossible 체크 후에 gameMap 호출해서 반영
	m_Map->GetInstance()->DrawLine(indexedPosition);

	//IsClosed()
	IndexedPosition tempArray[CHECKED_TILE_ARRAY_SIZE] = {{0}};
	
	if (IsClosed(indexedPosition, tempArray) )
	{
		int i = 0;

		while (tempArray[i].m_PosI != 0 && tempArray[i].m_PosJ != 0 )
		{
			//본래 타일에 뭐가 있었는지 확인해서 각자 바꿀 것!!
			switch ( m_Map->GetMapType(tempArray[i]) )
			{
				case MO_TILE_VOID:
					m_Map->GetInstance()->SetMapType( tempArray[i], MO_TILE_VOID_P1 );
					break;
				case MO_TILE_TRASH:
					m_Map->GetInstance()->SetMapType( tempArray[i], MO_TILE_TRASH_P1 );
					break;
				case MO_TILE_GOLD:
					m_Map->GetInstance()->SetMapType( tempArray[i], MO_TILE_GOLD_P1 );
					break;
			}
			i++;
		}
	}
}

//마우스 좌표값을 index로 바꾸는 함수
IndexedPosition CLogic::CalcualteIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;

	//마우스의 위치를 맵이 그려지는 기준점 좌표계를 기준으로 변환
	mouseCoordinate.m_PosX -= (int)(m_Map->GetInstance()->GetStartPosition().width);
	mouseCoordinate.m_PosY -= (int)(m_Map->GetInstance()->GetStartPosition().height);

	//타일 하나와 라인 하나를 묶어서 모듈러 연산으로 인덱스 값 계산
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
	//선택화면에서 플레이어 수를 선택!
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

//
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

bool CLogic::IsClosed( IndexedPosition indexedPosition, IndexedPosition* candidateTIleList )
{
	//선택된 울타리의 위쪽 확인
	if(ExploreTile(indexedPosition, candidateTIleList, DI_UP) )
		return true;

	//선택된 울타리의 오른쪽 확인
	if(ExploreTile(indexedPosition, candidateTIleList, DI_RIGHT) )
		return true;

	//선택된 울타리의 아래쪽 확인
	if(ExploreTile(indexedPosition, candidateTIleList, DI_DOWN) )
		return true;

	//선택된 울타리의 왼쪽 확인
	if(ExploreTile(indexedPosition, candidateTIleList, DI_LEFT) )
		return true;

	return false;
}

bool CLogic::IsAlreadyChecked(IndexedPosition* candidateTileList, IndexedPosition nextTile)
{
	int i = 0;

	while (candidateTileList[i].m_PosI != 0 && candidateTileList[i].m_PosJ != 0)
	{
		if (candidateTileList[i].m_PosI == nextTile.m_PosI && candidateTileList[i].m_PosJ == nextTile.m_PosJ)
		{
			return true;
		}

		i++;
	}

	return false;
}

bool CLogic::ExploreTile(IndexedPosition indexedPosition, IndexedPosition* candidateTIleList, Direction direction){
	std::queue<IndexedPosition> searchTiles;

	IndexedPosition currentTile = {0,0};
	IndexedPosition nextTile;

	//확인 할 방향을 지정
	switch (direction)
	{
	case DI_UP:
		currentTile.m_PosI = indexedPosition.m_PosI - 1;
		currentTile.m_PosJ = indexedPosition.m_PosJ;
		break;
	case DI_RIGHT:
		currentTile.m_PosI = indexedPosition.m_PosI;
		currentTile.m_PosJ = indexedPosition.m_PosJ + 1;
		break;
	case DI_DOWN:
		currentTile.m_PosI = indexedPosition.m_PosI + 1;
		currentTile.m_PosJ = indexedPosition.m_PosJ;
		break;
	case DI_LEFT:
		currentTile.m_PosI = indexedPosition.m_PosI;
		currentTile.m_PosJ = indexedPosition.m_PosJ - 1;
		break;
	default:
		break;
	}

	//확인 할 방향의 출발점이 점이면 확인 안 함
	if (m_Map->GetMapType(currentTile) == MO_DOT)
	{
		return false;
	}

	//앞에서 갱신한 탐색 출발 지점을 큐와 배열에 넣는다.
	int i = 0;

	searchTiles.push(currentTile);
	candidateTIleList[i++] = currentTile;
		
	while (!searchTiles.empty() )
	{
		currentTile = searchTiles.front();
		searchTiles.pop();

		//currentTile이 sentinel이면 지금까지 확인한 방향으로는 도형이 열려있으므로 확인한 타일을 저장하는 배열은 초기화하고 확인 종료
		if (CGameMap::GetInstance()->GetMapType(currentTile) == MO_SENTINEL)
		{
			memset(candidateTIleList, 0, sizeof(*candidateTIleList) * CHECKED_TILE_ARRAY_SIZE);

			/*
			//각각의 방향에서 큐를 새로 생성하므로 초기화 할 필요 없음
			while(!searchTiles.empty())
				searchTiles.pop();
			*/

			printf("센티넬을 만났어요\n");

			return false;
		}

		//현재 타일의 위쪽 확인
		if (CGameMap::GetInstance()->GetMapType(currentTile.m_PosI - 1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
		{

			nextTile.m_PosI = currentTile.m_PosI - 2;
			nextTile.m_PosJ = currentTile.m_PosJ;
			if (!IsAlreadyChecked(candidateTIleList, nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTIleList[i++] = nextTile;
			}				
		}

		//현재 타일의 오른쪽 확인
		if (CGameMap::GetInstance()->GetMapType(currentTile.m_PosI, currentTile.m_PosJ + 1) == MO_LINE_UNCONNECTED)
		{

			nextTile.m_PosI = currentTile.m_PosI;
			nextTile.m_PosJ = currentTile.m_PosJ + 2;
			if (!IsAlreadyChecked(candidateTIleList, nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTIleList[i++] = nextTile;
			}				
		}

		//현재 타일의 아래쪽 확인
		if (CGameMap::GetInstance()->GetMapType(currentTile.m_PosI + 1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
		{

			nextTile.m_PosI = currentTile.m_PosI + 2;
			nextTile.m_PosJ = currentTile.m_PosJ;
			if (!IsAlreadyChecked(candidateTIleList, nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTIleList[i++] = nextTile;
			}				
		}
		
		//현재 타일의 왼쪽 확인
		if (CGameMap::GetInstance()->GetMapType(currentTile.m_PosI, currentTile.m_PosJ - 1) == MO_LINE_UNCONNECTED)
		{

			nextTile.m_PosI = currentTile.m_PosI;
			nextTile.m_PosJ = currentTile.m_PosJ - 2;
			if (!IsAlreadyChecked(candidateTIleList, nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTIleList[i++] = nextTile;
			}				
		}
	}

	return true;
}