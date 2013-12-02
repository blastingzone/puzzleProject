#include "stdafx.h"
#include "PlayScene.h"
#include "GameTimer.h"
#include <queue>

CPlayScene::CPlayScene(void)
{
#ifdef _DEBUG
	AllocConsole();
	FILE* pFile;
	freopen_s(&pFile, "CONOUT$", "wb", stdout);
#endif // _DEBUG

	m_PlayerTurn = 0;
	m_Map = nullptr;
	m_PlayerNumber = 0;

	m_ClickBuffer = 0;
	m_ClickLineWeight = 0;
	m_ClickTileSize = 0;

	memset(m_Player,0,sizeof(m_Player) );

	m_SceneStatus = SC_PLAY;
}


CPlayScene::~CPlayScene(void)
{
	FreeConsole();
	RemoveObject();

	DeletePlayers();

	SafeDelete(m_Map);
}


bool CPlayScene::Init()
{
	SetPlayerNumber();
	CreatePlayers();
	m_Map = new CGameMap(CGameData::GetInstance()->GetMapSize());

	if ( m_Map == nullptr || !m_Map->Init() )
	{
		return false;
	}

	InitRandomMap();
	AddObject(m_Map);

	SetClickArea();
	CGameTimer::GetInstance()->SetTimerStart();

	PlayBGM();

	return true;
}

void CPlayScene::SetClickArea()
{
	m_ClickBuffer = SC_P_CLICK_BUFFER * CRenderer::GetInstance()->GetDisplayScale();
	m_ClickLineWeight = m_Map->GetLineWeight() + (m_ClickBuffer * 2);
	m_ClickTileSize = m_Map->GetTileSize() - (m_ClickBuffer * 2);
}

//지도 관련 정보를 업데이트 해주는 함수
void CPlayScene::EventHandle(Coordinate mouseCoordinate)
{
	//입력된 마우스 포인터 위치가 게임 맵 범위 안이고 애니메이션 재생 중이 아닐 때만 처리
	if (mouseCoordinate.m_PosX > m_Map->GetStartPosition().width - m_ClickBuffer
		&& mouseCoordinate.m_PosX < CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_Map->GetStartPosition().width + m_ClickBuffer
		&& mouseCoordinate.m_PosY > m_Map->GetStartPosition().height - m_ClickBuffer
		&& mouseCoordinate.m_PosY < CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height - m_Map->GetStartPosition().height + m_ClickBuffer
		&& !m_Map->GetLineAnimationFlag() && m_Map->GetTileAnimationTurnNumber() == 0)
	{
		IndexedPosition indexedPosition(CalculateIndex(mouseCoordinate) );
		EventHandle(indexedPosition);
	}
}

void CPlayScene::EventHandle(IndexedPosition indexedPosition)
{
	if (IsPossible(indexedPosition) )
	{
#ifdef _DEBUG
		printf("<<< ---- 현재 플레이어 : %d ---- >>>\n",(m_PlayerTurn%m_PlayerNumber));
		printf(" i : %d, j : %d\n",indexedPosition.m_PosI,indexedPosition.m_PosJ);
#endif
		//IsPossible 체크 후에 gameMap 호출해서 반영
		m_Map->DrawLine(indexedPosition);
		memset(m_ClosedTile, 0, sizeof(IndexedPosition) * CHECKLIST_LENGTH);

		if (IsClosed(indexedPosition))
		{
			int i = 0;
			while (m_ClosedTile[i].m_PosI != 0 && m_ClosedTile[i].m_PosJ != 0 )
			{
				//본래 타일에 뭐가 있었는지 확인해서 각자 바꿀 것!!
				//m_Map->SetMapOwner(tempArray[i],  m_Player[m_PlayerTurn%m_PlayerNumber] ) //지금 플레이어가 누군가
				m_Map->SetMapOwner(m_ClosedTile[i],  (MO_OWNER)m_Player[m_PlayerTurn%m_PlayerNumber]->GetPlayerId());
				m_Map->SubtractVoidCount();
				i++;
			}

			//tile animation 시작 설정
			m_Map->SetTileAnimationTurn(1);
#ifdef _DEBUG
			printf("우와! 플레이어 %d가 땅을 먹었다!\n",(m_PlayerTurn%m_PlayerNumber));
#endif
		}

		if (IsEnd() )
		{
			m_Map->WriteResult();
			CGameData::GetInstance()->SetCurrentScene( SC_RESULT );
		}

		//m_PlayerTurn++;
		CGameData::GetInstance()->isPlayerTurn(m_Player[m_PlayerTurn%m_PlayerNumber]->GetPlayerId(),false);
		CGameData::GetInstance()->isPlayerTurn(m_Player[m_PlayerTurn++%m_PlayerNumber]->GetPlayerId(),true);


		//조심해!!
		//애니메이션 끝나면 타이머 재시작하게 해줘야함
		//현재는 여기서 초기화 하고 애니메이션이 끝나고 다시 초기화 함(애니메이션이 최대 20초를 넘지 않아야 하는 문제 남아 있음)
		CGameTimer::GetInstance()->SetTimerStart();
	}
}

void CPlayScene::MouseOver(Coordinate mouseCoordinate)
{
	m_Map->InitVirtualLineState();

	//입력된 마우스 포인터 위치가 게임 맵 범위 안일 때만 처리
	if (mouseCoordinate.m_PosX > m_Map->GetStartPosition().width - m_ClickBuffer
		&& mouseCoordinate.m_PosX < CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width - m_Map->GetStartPosition().width + m_ClickBuffer
		&& mouseCoordinate.m_PosY > m_Map->GetStartPosition().height - m_ClickBuffer
		&& mouseCoordinate.m_PosY < CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height - m_Map->GetStartPosition().height + m_ClickBuffer)
	{
		IndexedPosition indexedPosition(CalculateIndex(mouseCoordinate) );

		if (IsPossible(indexedPosition) )
		{
			m_Map->ShowVirtualLine(indexedPosition);
		}
	}
}

void CPlayScene::TimeOut()
{
	// Test Code
#ifdef _DEBUG
	printf("time over\n");
	printf("random line\n");
#endif

	IndexedPosition RandomTargetPosition;

	//random line creation
	while (true)
	{
		// 울타리는 (2,1), (1,2) 부터 시작하므로
		RandomTargetPosition.m_PosI = rand() % MAX_MAP_HEIGHT + 1; 
		RandomTargetPosition.m_PosJ = rand() % MAX_MAP_WIDTH + 1;

		// 랜덤 값으로 뽑은 좌표가 MO_LINE_UNCONNECTED일 경우에
		if ( m_Map->GetMapType(RandomTargetPosition) == MO_LINE_UNCONNECTED )
		{
			if ( IsPossible(RandomTargetPosition) )
			{
				EventHandle(RandomTargetPosition);
				break;
			}
		}
	}
}

//마우스 좌표값을 index로 바꾸는 함수
IndexedPosition CPlayScene::CalculateIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;

	//마우스의 위치를 맵이 그려지는 기준점 좌표계를 기준으로 변환
	mouseCoordinate.m_PosX -= static_cast<int>(m_Map->GetStartPosition().width) - m_ClickBuffer;
	mouseCoordinate.m_PosY -= static_cast<int>(m_Map->GetStartPosition().height) - m_ClickBuffer;

	float indexUnit = m_ClickLineWeight + m_ClickTileSize;
	//타일 하나와 라인 하나를 묶어서 모듈러 연산으로 인덱스 값 계산
	indexedPosition.m_PosI = 
		( mouseCoordinate.m_PosY / static_cast<int> (indexUnit) ) * 2 
		+ ( ( mouseCoordinate.m_PosY % static_cast<int> (indexUnit) > m_ClickLineWeight) ? 2 : 1);
	indexedPosition.m_PosJ = 
		( mouseCoordinate.m_PosX / static_cast<int> (indexUnit) ) * 2 
		+ ( ( mouseCoordinate.m_PosX % static_cast<int> (indexUnit) > m_ClickLineWeight) ? 2 : 1);

	return indexedPosition;
}

void CPlayScene::SetPlayerNumber()
{
	//SettingScene에서의 플레이어 수를 받아온다.

	m_PlayerNumber = CGameData::GetInstance()->GetplayerNumber();
}

//플레이어 수에 맞춰 CPlayer 생성 후 m_Player에 넣는다. 순서는 입력 순서와 같다.
//여기서 순서까지 다 정해준다.
void CPlayScene::CreatePlayers()
{
	//조심해!!
	//세팅메뉴와 연결되는 부분은 수요일에 구현 예정.
	//일단 직접 집어 넣는 식으로 했음.
	CGameData::GetInstance()->SetPlayerIdAndName(0,L"Player1");
	CGameData::GetInstance()->SetPlayerIdAndName(1,L"Player2");
	CGameData::GetInstance()->SetPlayerIdAndName(2,L"Player3");
	CGameData::GetInstance()->SetPlayerIdAndName(3,L"Player4");
	
	for (int playerTurn = 0; playerTurn<m_PlayerNumber;++playerTurn)
	{
		int tempId;
		while (true)
		{
			tempId = rand()%m_PlayerNumber;
			bool flag = false;
			for(int j=0;j<playerTurn;++j)
			{
				if (tempId == m_Player[j]->GetPlayerId())
				{
					flag = true;
					break;
				}
			}
			if (!flag)
				break;
		}
		m_Player[playerTurn] = new CPlayer;
		m_Player[playerTurn]->SetPlayerId(tempId);
		m_Player[playerTurn]->SetPlayerName(CGameData::GetInstance()->GetPlayerName(tempId));
		CGameData::GetInstance()->SetPlayerTurn(tempId,playerTurn);
	}
}

void CPlayScene::DeletePlayers()
{
	for ( int i = 0; i< MAX_PLAYER_NUM ; ++i)
	{
		if (m_Player[i])
		{
			SafeDelete(m_Player[i]);
		}
	}
}

bool CPlayScene::IsClosed( IndexedPosition indexedPosition)
{

#ifdef _DEBUG
	printf("idx I : %d / idx J : %d\n", indexedPosition.m_PosI, indexedPosition.m_PosJ);
#endif

	//선택된 울타리의 위쪽 확인
	CollectClosedTile(indexedPosition,DI_UP);
	if (m_ClosedTile[0].m_PosI != 0 && m_ClosedTile[0].m_PosJ != 0)
		return true;

	//선택된 울타리의 오른쪽 확인
	CollectClosedTile(indexedPosition, DI_RIGHT);
	if (m_ClosedTile[0].m_PosI != 0 && m_ClosedTile[0].m_PosJ != 0)
		return true;

	//선택된 울타리의 아래쪽 확인
	CollectClosedTile(indexedPosition, DI_DOWN);
	if (m_ClosedTile[0].m_PosI != 0 && m_ClosedTile[0].m_PosJ != 0)
		return true;

	//선택된 울타리의 왼쪽 확인
	CollectClosedTile(indexedPosition, DI_LEFT);
	if (m_ClosedTile[0].m_PosI != 0 && m_ClosedTile[0].m_PosJ != 0)
		return true;

	return false;
}

bool CPlayScene::IsPossible(IndexedPosition indexedPosition)
{
	if (m_Map->GetMapType(indexedPosition) == MO_LINE_UNCONNECTED || m_Map->GetMapType(indexedPosition) == MO_LINE_HIDDEN)
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
	return m_Map->GetMapFlag(nextTile);
}

void CPlayScene::CollectClosedTile(IndexedPosition indexedPosition, Direction direction)
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
	if (m_Map->GetMapType(currentTile) != MO_DOT)
	{
		//타일에 애니메이션 적용하는 순서 나타내기 위한 변수
		int animationTurn = 1;
		m_Map->SetAnimationState(currentTile, animationTurn, direction);

		//앞에서 갱신한 탐색 출발 지점을 큐와 배열에 넣는다.
		int i = 0;

		searchTiles.push(currentTile);
		m_ClosedTile[i++] = currentTile;
		m_Map->SetMapFlag(currentTile, true);

		while (!searchTiles.empty() )
		{
			currentTile.m_PosI = searchTiles.front().m_PosI;
			currentTile.m_PosJ = searchTiles.front().m_PosJ;
			searchTiles.pop();

			//currentTile이 sentinel이면 지금까지 확인한 방향으로는 도형이 열려있으므로 확인한 타일을 저장하는 배열은 초기화하고 확인 종료
			if (m_Map->GetMapType(currentTile) == MO_SENTINEL)
			{
				for (int tempI = 0 ; tempI < MAX_MAP_WIDTH; ++tempI)
				{
					for (int tempJ = 0 ; tempJ < MAX_MAP_HEIGHT; ++tempJ)
					{
						m_Map->SetMapFlag(IndexedPosition(tempI, tempJ), false);

						if (m_Map->GetMapType(IndexedPosition(tempI, tempJ) ) == MO_TILE)
						{
							//애니메이션 재생을 위한 데이터도 초기화
							m_Map->InitAnimationState(IndexedPosition(tempI, tempJ) );
						}

						//재생할 애니메이션이 없으므로 0으로 설정
						animationTurn = 0;
					}
				}
				memset(m_ClosedTile, 0, sizeof(IndexedPosition) * CHECKLIST_LENGTH);
#ifdef _DEBUG
				printf("센티넬을 만났어요\n");
#endif
				break;
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
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);

					//애니메이션 재생을 위한 순서와 방향 지정
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_UP);
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
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);
					
					//애니메이션 재생을 위한 순서와 방향 지정
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_RIGHT);
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
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);
					
					//애니메이션 재생을 위한 순서와 방향 지정
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_DOWN);
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
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);
					
					//애니메이션 재생을 위한 순서와 방향 지정
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_LEFT);
				}				
			}
		}
		//닫힌 타일이 있으므로 애니메이션 실행하라는 플래그 설정
		m_Map->SetTileAnimationTurnNumber(animationTurn);
	}
}

// 초기에 맵에 랜덤으로 아이템과 울타리를 배치하는 함수입니다.
void CPlayScene::InitRandomMap()
{
	// 울타리, 금, 쓰레기의 초기값을 각각 설정해줍니다.
	int startLineNumber =	m_PlayerNumber * 5;
	int startGoldNumber =	m_PlayerNumber * 2;
	int startTrashNumber =	m_PlayerNumber * 2;

	// IsClosed에서 사용할 변수들입니다.
	IndexedPosition RandomTargetPosition;

	srand( static_cast<unsigned int>(time(NULL)) );

	// 고정적인 시드값이 필요할 경우 아래 시드를 써보시기 바랍니다.
	//srand(1383706550);

	// 울타리를 뿌려주는 함수입니다.
	while (startLineNumber)
	{
		// 울타리는 (2,1), (1,2) 부터 시작하므로
		RandomTargetPosition.m_PosI = rand() % MAX_MAP_HEIGHT + 1; 
		RandomTargetPosition.m_PosJ = rand() % MAX_MAP_WIDTH + 1;

		// 랜덤 값으로 뽑은 좌표가 MO_LINE_UNCONNECTED일 경우에
		if ( m_Map->GetMapType(RandomTargetPosition) == MO_LINE_UNCONNECTED )
		{
			// IsPossible을 만족하면
			if ( IsPossible(RandomTargetPosition) )
			{
				// 일단 그립니다(IsClosed 검사를 위해서)
				//printf("random %d , %d\n",RandomTargetPosition.m_PosI,RandomTargetPosition.m_PosJ);
				m_Map->DrawLine(RandomTargetPosition);
				--startLineNumber;

				// 지금 막 그려진 선이 IsClosed() 조건을 만족하지 않으면 그대로 종료
				if ( IsClosed(RandomTargetPosition) )
				{
					// 만약 지금 막 그려진 선이 어떤 도형을 닫는다면 해당 선을 삭제하고 라인 카운터를 복구
					m_Map->DeleteLine(RandomTargetPosition);
					++startLineNumber;
				}
			}
		}

	}

	// 타일 속성을 얘네들로 바꾸기 전에
	// 1. 브러쉬를 새로 준비해서 타일 위에 그려줘야 합니다
	// 2. 그리고 IsClosed 에서 잘 돌아가는지 한 번 테스트 해 보고 진짜 반영
	// 3. 얘네들은 이미지 파일로 넣어주는게 예쁠 듯 합니다

	while (startGoldNumber)
	{
		RandomTargetPosition.m_PosI = rand() % MAX_MAP_HEIGHT + 2;
		RandomTargetPosition.m_PosJ = rand() % MAX_MAP_WIDTH + 2;
		if (m_Map->GetMapType(RandomTargetPosition) == MO_TILE 
			&& m_Map->GetItem(RandomTargetPosition) == MO_NOTHING)
		{
			m_Map->SetItem(RandomTargetPosition, MO_GOLD);
			startGoldNumber--;
		}
	}

	while (startTrashNumber)
	{
		RandomTargetPosition.m_PosI = rand() % MAX_MAP_HEIGHT + 2;
		RandomTargetPosition.m_PosJ = rand() % MAX_MAP_WIDTH + 2;
		if (m_Map->GetMapType(RandomTargetPosition) == MO_TILE 
			&& m_Map->GetItem(RandomTargetPosition) == MO_NOTHING)
		{
			m_Map->SetItem(RandomTargetPosition, MO_TRASH);
			startTrashNumber--;
		}
	}

	return;
}

bool CPlayScene::IsEnd()
{
	return (m_Map->GetVoidTileCount() == 0);
}


void CPlayScene::Render()
{
	for (auto iter: m_Object)
	{
		iter->Render();
	}

	CGameTimer::GetInstance()->Update();
	//timer 여기에 추가할 것
	CGameTimer::GetInstance()->Render();

	if (CGameData::GetInstance()->GetPlaySceneTimerFlag() )
	{
		TimeOut();
		CGameData::GetInstance()->InitPlaySceneTimerFlag();
	}

#ifdef _DEBUG		
	CFPS::GetInstance()->Update();
	CFPS::GetInstance()->Render();
#endif
}

void CPlayScene::PlayBGM()
{
	CSoundRenderer::GetInstance()->PlayBGM(BGM_PLAY);
}

void CPlayScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}

void CPlayScene::ResizeClient()
{
	for (auto iter: m_Object)
	{
		iter->ResizeClient();
	}

	SetClickArea();
}