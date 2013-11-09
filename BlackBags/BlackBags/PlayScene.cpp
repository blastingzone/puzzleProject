#include "stdafx.h"
#include "PlayScene.h"
#include "MacroSet.h"
#include <queue>

//CLogic* CLogic::m_pInstance = nullptr;

CPlayScene::CPlayScene(void)
{
	AllocConsole();
	FILE* pFile;
	freopen_s(&pFile, "CONOUT$", "wb", stdout);

	m_PlayerTurn = 0;
	m_Map = nullptr;

	Init();
	m_SceneStatus = SC_PLAY;
	AddObject(m_Map);
}


CPlayScene::~CPlayScene(void)
{
	Release();
	delete m_Map;
}
/*
CLogic* g_Logic
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLogic();
	}

	return m_pInstance;
}
*/
bool CPlayScene::Release()
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
/*
bool CLogic::ReleaseInstance()
{
	m_pInstance->Release();
	delete m_pInstance;

	return true;
}
*/

//g_Logic관련 초기화 함수
void CPlayScene::Init()
{
	memset(m_Player,0,sizeof(m_Player));
	GetPlayerNumber();
	CreatePlayers();
	SetPlayerTurn();

	m_Map = new CGameMap();
	m_Map->Init();
	InitRandomMap();
}

//지도 관련 정보를 업데이트 해주는 함수
SceneName CPlayScene::Update( Coordinate mouseCoordinate )
{
	
	IndexedPosition indexedPosition(CalculateIndex(mouseCoordinate) );
	if (!IsPossible(indexedPosition) )
	{
		return SC_PLAY;
	}

#ifdef _DEBUG
	printf("<<< ---- 현재 플레이어 : %d ---- >>>\n",(m_PlayerTurn%m_PlayerNumber));
	printf(" i : %d, j : %d\n",indexedPosition.m_PosI,indexedPosition.m_PosJ);
#endif  

	//IsPossible 체크 후에 gameMap 호출해서 반영
	m_Map->DrawLine(indexedPosition);

	//IsClosed()
	IndexedPosition tempArray[CHECKED_TILE_ARRAY_SIZE];
	
	if (IsClosed(indexedPosition, tempArray) )
	{
		int i = 0;

		while (tempArray[i].m_PosI != 0 && tempArray[i].m_PosJ != 0 )
		{
			//본래 타일에 뭐가 있었는지 확인해서 각자 바꿀 것!!
			//m_Map->SetMapOwner(tempArray[i],  m_Player[m_PlayerTurn%m_PlayerNumber] ) //지금 플레이어가 누군가
			m_Map->SetMapOwner(tempArray[i],  (MO_OWNER)m_Player[m_PlayerTurn%m_PlayerNumber]->GetPlayerId());
			m_Map->SubtractVoidCount();
			i++;
		}
#ifdef _DEBUG
		printf("우와! 플레이어 %d가 땅을 먹었다!\n",(m_PlayerTurn%m_PlayerNumber));
#endif
	}
	m_PlayerTurn++;

	if (IsEnd() )
	{
		return SC_RESULT;
	}

	return SC_PLAY;
}


//마우스 좌표값을 index로 바꾸는 함수
IndexedPosition CPlayScene::CalculateIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;

	//마우스의 위치를 맵이 그려지는 기준점 좌표계를 기준으로 변환
	mouseCoordinate.m_PosX -= static_cast<int>(m_Map->GetStartPosition().width);
	mouseCoordinate.m_PosY -= static_cast<int>(m_Map->GetStartPosition().height);

	//타일 하나와 라인 하나를 묶어서 모듈러 연산으로 인덱스 값 계산
	indexedPosition.m_PosI = 
		( mouseCoordinate.m_PosY / static_cast<int> (TILE_SIZE + LINE_WEIGHT) ) * 2 
		+ ( ( mouseCoordinate.m_PosY % static_cast<int> (TILE_SIZE + LINE_WEIGHT) > LINE_WEIGHT ) ? 2 : 1);
	indexedPosition.m_PosJ = 
		( mouseCoordinate.m_PosX / static_cast<int> (TILE_SIZE + LINE_WEIGHT) ) * 2 
		+ ( ( mouseCoordinate.m_PosX % static_cast<int> (TILE_SIZE + LINE_WEIGHT) > LINE_WEIGHT ) ? 2 : 1);
	
	return indexedPosition;
}

bool CPlayScene::GetPlayerNumber()
{
	//선택화면에서 플레이어 수를 선택!
	//조심해!! - 나중에 플레이어 수 입력 받으면 바꿔주는 걸로 수정할 것
	m_PlayerNumber = 4;

	return true;
}

bool CPlayScene::CreatePlayers()
{
	for (int i = 0; i<m_PlayerNumber;++i)
	{
		m_Player[i] = new CPlayer();

		m_Player[i]->SetPlayerName("hihihi");
		m_Player[i]->SetPlayerTurn(-1);
		m_Player[i]->SetPlayerId(i);
	}

	return true;
}

bool CPlayScene::SetPlayerTurn()
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

bool CPlayScene::IsClosed( IndexedPosition indexedPosition, OUT IndexedPosition* candidateTileList )
{
	
#ifdef _DEBUG
	printf("idx I : %d / idx J : %d\n", indexedPosition.m_PosI, indexedPosition.m_PosJ);
#endif

	//선택된 울타리의 위쪽 확인
	if (ExploreTile(indexedPosition, candidateTileList, DI_UP) )
		return true;

	//선택된 울타리의 오른쪽 확인
	if (ExploreTile(indexedPosition, candidateTileList, DI_RIGHT) )
		return true;

	//선택된 울타리의 아래쪽 확인
	if (ExploreTile(indexedPosition, candidateTileList, DI_DOWN) )
		return true;

	//선택된 울타리의 왼쪽 확인
	if (ExploreTile(indexedPosition, candidateTileList, DI_LEFT) )
		return true;

	return false;
}

bool CPlayScene::IsPossible(IndexedPosition indexedPosition)
{
	if (m_Map->GetMapType(indexedPosition.m_PosI, indexedPosition.m_PosJ) == MO_LINE_UNCONNECTED)
	{
		int tileVoidCount = 0;

		//입력된 울타리 주변을 확인해서 소유주가 없는 타일과 센티널의 숫자를 센다
		if (m_Map->GetMapOwner(indexedPosition.m_PosI + 1, indexedPosition.m_PosJ) == MO_NOBODY) { ++tileVoidCount; }

		if (m_Map->GetMapOwner(indexedPosition.m_PosI - 1, indexedPosition.m_PosJ) == MO_NOBODY) { ++tileVoidCount; }

		if (m_Map->GetMapOwner(indexedPosition.m_PosI, indexedPosition.m_PosJ + 1) == MO_NOBODY) { ++tileVoidCount; }

		if (m_Map->GetMapOwner(indexedPosition.m_PosI, indexedPosition.m_PosJ - 1) == MO_NOBODY) { ++tileVoidCount; }
		
		//확인된 타일의 수가 4가 되면 입력된 울타리는 열린 타일들 사이에 있으므로 그을 수 있음??
		if (tileVoidCount == 4)
		{
			return true;
		}
	}

	return false;
}

bool CPlayScene::IsAlreadyChecked(const IndexedPosition& nextTile)
{
// 	int i = 0;
// 
// 	while (candidateTileList[i].m_PosI != 0 && candidateTileList[i].m_PosJ != 0)
// 	{
// 		if (candidateTileList[i].m_PosI == nextTile.m_PosI && candidateTileList[i].m_PosJ == nextTile.m_PosJ)
// 		{
// 			return true;
// 		}
// 
// 		i++;
// 	}

	return m_Map->GetMapFlag(nextTile);
}

bool CPlayScene::ExploreTile(IndexedPosition indexedPosition, OUT IndexedPosition* candidateTileList, Direction direction)
{
	std::queue<IndexedPosition> searchTiles;

	IndexedPosition currentTile;
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
	candidateTileList[i++] = currentTile;
	m_Map->SetMapFlag(currentTile, true);
		
	while (!searchTiles.empty() )
	{
		currentTile.m_PosI = searchTiles.front().m_PosI;
		currentTile.m_PosJ = searchTiles.front().m_PosJ;
		searchTiles.pop();

		//currentTile이 sentinel이면 지금까지 확인한 방향으로는 도형이 열려있으므로 확인한 타일을 저장하는 배열은 초기화하고 확인 종료
		if (m_Map->GetMapType(currentTile) == MO_SENTINEL)
		{
			/*
			int checkIdx = 0;
			while (candidateTileList[checkIdx].m_PosI != 0 && candidateTileList[checkIdx].m_PosJ != 0)
			{
				m_Map->SetMapFlag(candidateTileList[checkIdx], false);
				checkIdx++;
			}
			*/
			for (int tempI = 0 ; tempI < MAX_WIDTH; ++tempI){
				for (int tempJ = 0 ; tempJ < MAX_HEIGHT; ++tempJ){
					m_Map->SetMapFlag(IndexedPosition(tempI, tempJ), false);
				}
			}
			memset(candidateTileList, 0, sizeof(IndexedPosition) * CHECKED_TILE_ARRAY_SIZE);
			
			/*
			int checkIdx = 0;
			while (m_Map->GetMapFlag(candidateTileList[checkIdx]) )
			{
				m_Map->SetMapFlag(candidateTileList[checkIdx], false);
			}
			memset(candidateTileList, 0, sizeof(IndexedPosition) * checkIdx);
			
			//각각의 방향에서 큐를 새로 생성하므로 초기화 할 필요 없음
			while (!searchTiles.empty())
				searchTiles.pop();
			*/
#ifdef _DEBUG
			printf("센티넬을 만났어요\n");
#endif
			return false;
		}
#ifdef _DEBUG
			printf("idx I : %d / idx J : %d\n", currentTile.m_PosI, currentTile.m_PosJ);
#endif
		//현재 타일의 위쪽 확인
		if (m_Map->GetMapType(currentTile.m_PosI - 1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
		{
			nextTile.m_PosI = currentTile.m_PosI - 2;
			nextTile.m_PosJ = currentTile.m_PosJ;
			if (!IsAlreadyChecked(nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTileList[i++] = nextTile;
				m_Map->SetMapFlag(nextTile, true);
			}				
		}

		//현재 타일의 오른쪽 확인
		if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ + 1) == MO_LINE_UNCONNECTED)
		{
			nextTile.m_PosI = currentTile.m_PosI;
			nextTile.m_PosJ = currentTile.m_PosJ + 2;
			if (!IsAlreadyChecked(nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTileList[i++] = nextTile;
				m_Map->SetMapFlag(nextTile, true);
			}				
		}

		//현재 타일의 아래쪽 확인
		if (m_Map->GetMapType(currentTile.m_PosI + 1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
		{
			nextTile.m_PosI = currentTile.m_PosI + 2;
			nextTile.m_PosJ = currentTile.m_PosJ;
			if (!IsAlreadyChecked(nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTileList[i++] = nextTile;
				m_Map->SetMapFlag(nextTile, true);
			}				
		}
		
		//현재 타일의 왼쪽 확인
		if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ - 1) == MO_LINE_UNCONNECTED)
		{
			nextTile.m_PosI = currentTile.m_PosI;
			nextTile.m_PosJ = currentTile.m_PosJ - 2;
			if (!IsAlreadyChecked(nextTile) )
			{
				searchTiles.push(nextTile);
				candidateTileList[i++] = nextTile;
				m_Map->SetMapFlag(nextTile, true);
			}				
		}
	}

	return true;
}

void CPlayScene::InitRandomMap()
{
	int startLineNumber =	m_PlayerNumber * 5;
//	int startGoldNumber =	m_PlayerNumber * 2;
//	int startTrashNumber =	m_PlayerNumber * 2;
	
	IndexedPosition RandomTargetPosition;
	IndexedPosition checkList[100];

	//srand( static_cast<unsigned int>(time(NULL)) );
	
	// 버그 발생하는 시드값을 넣어두겠습니다
	srand(1383706550);

	while (startLineNumber)
	{
		RandomTargetPosition.m_PosI = rand() % MAX_HEIGHT + 2; 
		RandomTargetPosition.m_PosJ = rand() % MAX_WIDTH + 2;

		if ( m_Map->GetMapType(RandomTargetPosition) == MO_LINE_UNCONNECTED )
			if ( IsPossible(RandomTargetPosition) )
				{
					//printf("random %d , %d\n",RandomTargetPosition.m_PosI,RandomTargetPosition.m_PosJ);
					m_Map->DrawLine(RandomTargetPosition);
					--startLineNumber;
					
					if ( IsClosed(RandomTargetPosition, checkList) )
					{
						m_Map->DeleteLine(RandomTargetPosition);
						++startLineNumber;
					}
				}
				
	}

	// 타일 속성을 얘네들로 바꾸기 전에
	// 1. 브러쉬를 새로 준비하고
	// 2. IsClosed() 로직에 얘네들도 반영해야됩니다 ㄷㄷ;
	// 지금은 그냥 돌려버리면 IsClosed에서 길을 못 찾아감;

	//while (startGoldNumber)
	//{
	//	RandomTargetPosition.m_PosI = rand() % MAX_HEIGHT + 2;
	//	RandomTargetPosition.m_PosJ = rand() % MAX_WIDTH + 2;
	//	if (m_Map->GetMapType(RandomTargetPosition) == MO_TILE_VOID)
	//	{
	//		m_Map->SetMapType(RandomTargetPosition, MO_TILE_GOLD);
	//		startGoldNumber--;
	//	}
	//}

	//while (startTrashNumber)
	//{
	//	RandomTargetPosition.m_PosI = rand() % MAX_HEIGHT + 2;
	//	RandomTargetPosition.m_PosJ = rand() % MAX_WIDTH + 2;
	//	if (m_Map->GetMapType(RandomTargetPosition) == MO_TILE_VOID)
	//	{
	//		m_Map->SetMapType(RandomTargetPosition, MO_TILE_TRASH);
	//		startTrashNumber--;
	//	}
	//}

	return;
}

bool CPlayScene::IsEnd()
{
	return (m_Map->GetVoidTileCount() == 0);
}