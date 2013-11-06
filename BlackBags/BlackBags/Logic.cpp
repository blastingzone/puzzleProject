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
	m_PlayerTurn = 0;
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
	InitRandomMap();
}

//지도 관련 정보를 업데이트 해주는 함수
void CLogic::Update( Coordinate mouseCoordinate )
{
	printf("<<< ---- 현재 플레이어 : %d ---- >>>\n",(m_PlayerTurn%m_PlayerNumber));
	IndexedPosition indexedPosition; 
	indexedPosition = CalcualteIndex(mouseCoordinate); //SM9: 이렇게 객체를 그냥 대입 하려면 복사대입 생성자 만들어 놓는게 좋다.

	//SM9: 콘솔창 찍는거는 디버그일때만 쓰는거니까 이런식으로 감싸줘서 release모드에서는 동작하지 않도록.
#ifdef _DEBUG
	printf(" i : %d, j : %d\n",indexedPosition.m_PosI,indexedPosition.m_PosJ);
#endif	
	//IsPossible 체크 후에 gameMap 호출해서 반영
	m_Map->GetInstance()->DrawLine(indexedPosition);

	//IsClosed()
	IndexedPosition tempArray[CHECKED_TILE_ARRAY_SIZE] ; 
	
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
		printf("우와! 플레이어 %d가 땅을 먹었다!\n",(m_PlayerTurn%m_PlayerNumber));
	}
	m_PlayerTurn++;
}

//마우스 좌표값을 index로 바꾸는 함수
IndexedPosition CLogic::CalcualteIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;

	//마우스의 위치를 맵이 그려지는 기준점 좌표계를 기준으로 변환
	mouseCoordinate.m_PosX -= (int)(m_Map->GetInstance()->GetStartPosition().width);
	mouseCoordinate.m_PosY -= (int)(m_Map->GetInstance()->GetStartPosition().height);

	//타일 하나와 라인 하나를 묶어서 모듈러 연산으로 인덱스 값 계산
	//SM9: C++에서는 웬만해서는 (거시기) 형태의 형변환은 쓰지 말 것. static_cast<int>(거시기) 처럼 써라.
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
	//SM9: 아래 상수 박아 놓은건 나중에 고칠거지?
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

bool CLogic::IsClosed( IndexedPosition indexedPosition, IndexedPosition* candidateTIleList )
{
	//선택된 울타리의 위쪽 확인
	if (ExploreTile(indexedPosition, candidateTIleList, DI_UP) )
		return true;

	//선택된 울타리의 오른쪽 확인
	if (ExploreTile(indexedPosition, candidateTIleList, DI_RIGHT) )
		return true;

	//선택된 울타리의 아래쪽 확인
	if (ExploreTile(indexedPosition, candidateTIleList, DI_DOWN) )
		return true;

	//선택된 울타리의 왼쪽 확인
	if (ExploreTile(indexedPosition, candidateTIleList, DI_LEFT) )
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

bool CLogic::ExploreTile(IndexedPosition indexedPosition, IndexedPosition* candidateTIleList, Direction direction){ //SM9: { } 위치는 항상 동일 컬럼에 정렬.

	std::queue<IndexedPosition> searchTiles;

	IndexedPosition currentTile ;
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
		currentTile = searchTiles.front(); //SM9: 사실 이렇게 객체를 copy하는것은 좋은 습관은 아니다. 다행히 IndexedPosition의 크기가 작아서 허용함.
		searchTiles.pop();

		//currentTile이 sentinel이면 지금까지 확인한 방향으로는 도형이 열려있으므로 확인한 타일을 저장하는 배열은 초기화하고 확인 종료
		if (CGameMap::GetInstance()->GetMapType(currentTile) == MO_SENTINEL)
		{
			//SM9: 위험한 코딩... 계산 잘못하면 메모리 덮어씀... 이럴때는 sizeof(*candidateTIleList) 이것 보다 sizeof(IndexedPosition)로 타입을 명확히 쓸 것
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

			//SM9: 아래의 IsAlreadyChecked안에서 candidateTIleList의 주소값을 주고 또 루프를 도는데, 더 좋은 방법은 없을까?
			// 그리고 아래 함수 안에서 candidateTIleList의 변경이 없다면 반드시 const IndexedPosition& 형태로 넘겨라.
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

void CLogic::InitRandomMap()
{
	int startLineNumber =	m_PlayerNumber * 5;
//	int startGoldNumber =	m_PlayerNumber * 2;
//	int startTrashNumber =	m_PlayerNumber * 2;
	
	IndexedPosition RandomTargetPosition;
	IndexedPosition checkList[100] ;

	srand( (unsigned int)time(NULL) );
	
	while (startLineNumber)
	{
		RandomTargetPosition.m_PosI = rand() % MAX_HEIGHT + 2; 
		RandomTargetPosition.m_PosJ = rand() % MAX_WIDTH + 2;

		if ( m_Map->GetMapType(RandomTargetPosition) == MO_LINE_UNCONNECTED  //SM9: if 키워드 위에 ( 는 반드시 한칸 띄우고
			&& ( m_Map->IsPossible(RandomTargetPosition)  //SM9: 이렇게 조건문이 복잡할 경우는 라인을 바꿔서 읽기 좋게.
			&& !IsClosed(RandomTargetPosition, checkList) )
			)
		{
			//printf("random %d , %d\n",RandomTargetPosition.m_PosI,RandomTargetPosition.m_PosJ);
			m_Map->SetMapType(RandomTargetPosition, MO_LINE_CONNECTED);
			startLineNumber--;
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
	//	if(m_Map->GetMapType(RandomTargetPosition) == MO_TILE_VOID)
	//	{
	//		m_Map->SetMapType(RandomTargetPosition, MO_TILE_GOLD);
	//		startGoldNumber--;
	//	}
	//}

	//while (startTrashNumber)
	//{
	//	RandomTargetPosition.m_PosI = rand() % MAX_HEIGHT + 2;
	//	RandomTargetPosition.m_PosJ = rand() % MAX_WIDTH + 2;
	//	if(m_Map->GetMapType(RandomTargetPosition) == MO_TILE_VOID)
	//	{
	//		m_Map->SetMapType(RandomTargetPosition, MO_TILE_TRASH);
	//		startTrashNumber--;
	//	}
	//}

	return; //SM9: 멍미
}