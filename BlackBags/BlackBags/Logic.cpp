#include "stdafx.h"
#include "Logic.h"
#include "MacroSet.h"
#include <queue>

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

	//IsPossible 체크 후에 gameMap 호출해서 반영
	m_Map->GetInstance()->DrawLine(indexedPosition);

	//IsClosed()
	IndexedPosition tempArray[100] = {{0}};
	
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
	int i = 0;
	bool flag = true;
	std::queue<IndexedPosition> searchTiles;
	
	//바로 아래가 타일인 경우. 즉 선이 누워있는 경우.
	if (m_Map->GetMapType(indexedPosition.m_PosI+1,indexedPosition.m_PosJ) != MO_DOT)
	{
		IndexedPosition currentTile;
		IndexedPosition nextTile;
		// ++ == tile , 큐에 넣는다
		++indexedPosition.m_PosI;
		searchTiles.push(indexedPosition);
		//임시 배열에도 저장한다
		candidateTIleList[i++] = indexedPosition;
		
		while (!searchTiles.empty() )
		{
			currentTile = searchTiles.front();
			searchTiles.pop();
			if (m_Map->GetMapType(currentTile) == MO_SENTINEL)
			{
				memset(candidateTIleList, 0, sizeof(candidateTIleList));
				flag = false;
				break;
			}
			//////////////////////////////////
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ-1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ-2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI+1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI+2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ+1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ+2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI-1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI-2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}

		}
		if (flag)
		{
			return true;
		}
		flag = true;
	}


	//위쪽이 타일인 경우, 즉 선이 누워있는 경우
	if (m_Map->GetMapType(indexedPosition.m_PosI-1,indexedPosition.m_PosJ) != MO_DOT)
	{
		IndexedPosition currentTile;
		IndexedPosition nextTile;
		// ++ == tile , 큐에 넣는다
		--indexedPosition.m_PosI;
		searchTiles.push(indexedPosition);
		//임시 배열에도 저장한다
		candidateTIleList[i++] = indexedPosition;

		while (!searchTiles.empty() )
		{
			currentTile = searchTiles.front();
			searchTiles.pop();
			if (m_Map->GetMapType(currentTile) == MO_SENTINEL)
			{
				memset(candidateTIleList, 0, sizeof(candidateTIleList));
				flag = false;
				break;
			}
			//////////////////////////////////
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ-1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ-2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI+1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI+2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ+1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ+2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI-1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI-2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}

		}
		if (flag)
		{
			return true;
		}
		flag = true;
	}

	//오른쪽이 타일인 선이 서있는 경우
	if (m_Map->GetMapType(indexedPosition.m_PosI,indexedPosition.m_PosJ+1) != MO_DOT)
	{
		IndexedPosition currentTile;
		IndexedPosition nextTile;
		// ++ == tile , 큐에 넣는다
		++indexedPosition.m_PosJ;
		searchTiles.push(indexedPosition);
		//임시 배열에도 저장한다
		candidateTIleList[i++] = indexedPosition;

		while (!searchTiles.empty() )
		{
			currentTile = searchTiles.front();
			searchTiles.pop();
			if (m_Map->GetMapType(currentTile) == MO_SENTINEL)
			{
				memset(candidateTIleList, 0, sizeof(candidateTIleList));
				flag = false;
				break;
			}
			//////////////////////////////////
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ-1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ-2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI+1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI+2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ+1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ+2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI-1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI-2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}

		}
		if (flag)
		{
			return true;
		}
		flag = true;
	}

	//왼쪽이 타일인, 선이 서있는 경우
	if (m_Map->GetMapType(indexedPosition.m_PosI,indexedPosition.m_PosJ-1) != MO_DOT)
	{
		IndexedPosition currentTile;
		IndexedPosition nextTile;
		// ++ == tile , 큐에 넣는다
		--indexedPosition.m_PosJ;
		searchTiles.push(indexedPosition);
		//임시 배열에도 저장한다
		candidateTIleList[i++] = indexedPosition;

		while (!searchTiles.empty() )
		{
			currentTile = searchTiles.front();
			searchTiles.pop();
			if (m_Map->GetMapType(currentTile) == MO_SENTINEL)
			{
				memset(candidateTIleList, 0, sizeof(candidateTIleList));
				flag = false;
				break;
			}
			//////////////////////////////////
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ-1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ-2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI+1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI+2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ+1) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ+2;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}
			if (m_Map->GetMapType(currentTile.m_PosI-1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{

				nextTile.m_PosI = currentTile.m_PosI-2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(candidateTIleList, nextTile) )
				{
					searchTiles.push(nextTile);
					candidateTIleList[i++] = nextTile;
				}				
			}

		}
		if (flag)
		{
			return true;
		}
		flag = true;
	}

	return false;
}

bool CLogic::IsAlreadyChecked(IndexedPosition* candidateTileList, IndexedPosition nextTile)
{
	int i = 0;

	while (candidateTileList[i].m_PosI != 0 && candidateTileList[i].m_PosJ != 0)
	{
		i++;
		if (candidateTileList[i].m_PosI == nextTile.m_PosI && candidateTileList[i].m_PosJ == nextTile.m_PosJ)
		{
			return true;
		}
	}

	return false;
}

