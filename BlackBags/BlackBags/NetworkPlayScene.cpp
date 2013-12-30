#include "stdafx.h"
#include "NetworkPlayScene.h"
#include "NetworkGameTimer.h"
#include "NetworkManager.h"
#include "PacketType.h"
#include <queue>
#include <array>

CNetworkPlayScene::CNetworkPlayScene(void)
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

	m_EndFlag = false;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_Player[i] = nullptr;
	}

	m_SceneStatus = SC_NETWORK_PLAY;
}


CNetworkPlayScene::~CNetworkPlayScene(void)
{
	FreeConsole();
	RemoveObject();

	SafeDelete(m_Map);
}


bool CNetworkPlayScene::Init()
{
	SetPlayerNumber();

	m_Map = new CNetworkGameMap(CGameData::GetInstance()->GetMapSize());

	if ( m_Map == nullptr || !m_Map->Init() )
	{
		return false;
	}

	LinkPlayers();

	InitRandomMap();
	AddObject(m_Map);

	SetClickArea();
	//CNetworkGameTimer::GetInstance()->SetTimerStart();

	// 서버에 Ready 상태 보고
	TurnReadyRequest sendData;
	sendData.mClientId = CNetworkManager::GetInstance()->GetClientId();

	if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&sendData, sendData.mSize) )
	{
		CNetworkManager::GetInstance()->PostSendMessage();
	}

	PlayBGM();

	return true;
}

void CNetworkPlayScene::SetClickArea()
{
	m_ClickBuffer = SC_P_CLICK_BUFFER * CRenderer::GetInstance()->GetDisplayScale();
	m_ClickLineWeight = m_Map->GetLineWeight() + (m_ClickBuffer * 2);
	m_ClickTileSize = m_Map->GetTileSize() - (m_ClickBuffer * 2);
}

//지도 관련 정보를 업데이트 해주는 함수
void CNetworkPlayScene::EventHandle(Coordinate mouseCoordinate)
{

	if (m_EndFlag)
	{
		m_Map->WriteResult();
		CGameData::GetInstance()->SetCurrentScene( SC_NETWORK_RESULT );
		
		return;
	}
	
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

void CNetworkPlayScene::EventHandle(IndexedPosition indexedPosition)
{
	int clientIdIdx = CNetworkManager::GetInstance()->GetCurrentTurnId();

	// 현재 턴의 ClientId가 내 ClientId와 다르면 튕겨낸다
	// 내 턴일 때 IsPossible 이면 서버로 보내고, 패킷을 받으면 DrawLine을 실행한다.
	if (clientIdIdx != CNetworkManager::GetInstance()->GetClientId() )
		return;

	if (IsPossible(indexedPosition))
	{
		/*CSoundRenderer::GetInstance()->PlaySE_DrawLine();*/

		assert(m_PlayerNumber>0);

#ifdef _DEBUG
		printf("<<< ---- 현재 플레이어 : %d ---- >>>\n",(clientIdIdx));
		printf(" i : %d, j : %d\n",indexedPosition.m_PosI, indexedPosition.m_PosJ);
#endif
		EventPositionRequest sendData;
		sendData.mPlayerId = CNetworkManager::GetInstance()->GetClientId();
		sendData.m_Xpos = indexedPosition.m_PosI;
		sendData.m_Ypos = indexedPosition.m_PosJ;

		if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&sendData, sendData.mSize) )
		{
			CNetworkManager::GetInstance()->PostSendMessage();
		}
	}
}

void CNetworkPlayScene::MouseOver(Coordinate mouseCoordinate)
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

void CNetworkPlayScene::TimeOut()
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
				EventPositionRequest sendData;
				sendData.mPlayerId = CNetworkManager::GetInstance()->GetClientId();
				sendData.m_Xpos = RandomTargetPosition.m_PosI;
				sendData.m_Ypos = RandomTargetPosition.m_PosJ;

				if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&sendData, sendData.mSize) )
				{
					CNetworkManager::GetInstance()->PostSendMessage();
				}
				break;
			}
		}
	}
}

//마우스 좌표값을 index로 바꾸는 함수
IndexedPosition CNetworkPlayScene::CalculateIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;

	//마우스의 위치를 맵이 그려지는 기준점 좌표계를 기준으로 변환
	mouseCoordinate.m_PosX -= static_cast<int>(m_Map->GetStartPosition().width) - m_ClickBuffer;
	mouseCoordinate.m_PosY -= static_cast<int>(m_Map->GetStartPosition().height) - m_ClickBuffer;

	float indexUnit = m_ClickLineWeight + m_ClickTileSize;
	//타일 하나와 라인 하나를 묶어서 모듈러 연산으로 인덱스 값 계산
	if (indexUnit > 0)
	{
		indexedPosition.m_PosI = 
			( mouseCoordinate.m_PosY / static_cast<int> (indexUnit) ) * 2 
			+ ( ( mouseCoordinate.m_PosY % static_cast<int> (indexUnit) > m_ClickLineWeight) ? 2 : 1);
		indexedPosition.m_PosJ = 
			( mouseCoordinate.m_PosX / static_cast<int> (indexUnit) ) * 2 
			+ ( ( mouseCoordinate.m_PosX % static_cast<int> (indexUnit) > m_ClickLineWeight) ? 2 : 1);
	}

	return indexedPosition;
}

void CNetworkPlayScene::SetPlayerNumber()
{
	//SettingScene에서의 플레이어 수를 받아온다.
	m_PlayerNumber = CGameData::GetInstance()->GetplayerNumber();
}

void CNetworkPlayScene::LinkPlayers()
{
	std::array<int, MAX_PLAYER_NUM> PlayerTurn = {0, 1, 2, 3};

	//srand( static_cast<unsigned int>(time(NULL)) );
	// 서버에서 받아온 랜덤시드로 대동단결!
	srand(CGameData::GetInstance()->GetNetworkRandomSeed() );
	//std::random_shuffle(PlayerTurn.begin(), PlayerTurn.end());

	//player turn 설정
// 	int joinPlayerIdx = 0;
// 	int notJoinPlayerIdx = m_PlayerNumber;
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		int clientIdx = CNetworkManager::GetInstance()->GetCharacterClientId(i);
		if (clientIdx != -1 )
		{
			m_Player[clientIdx] = CGameData::GetInstance()->GetPlayerPtr( PlayerTurn[i] );
			m_Map->SetPlayerTurnTable(clientIdx, CGameData::GetInstance()->GetPlayerPtr( PlayerTurn[i]) );
			CGameData::GetInstance()->SetNetworkClientList(clientIdx, m_Player[clientIdx]);
		}
		else
		{
			//이미 조건에 clientIdx = -1이라는 건데 그걸로 배열을 접근하니까 에러가 나지요 ㅋㅋ
			//m_Player[clientIdx] = nullptr;
			//m_Map->SetPlayerTurnTable( clientIdx, nullptr );
		}
	}
}

bool CNetworkPlayScene::IsClosed( IndexedPosition indexedPosition)
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

bool CNetworkPlayScene::IsPossible(IndexedPosition indexedPosition)
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

bool CNetworkPlayScene::IsAlreadyChecked(const IndexedPosition& nextTile)
{
	return m_Map->GetMapFlag(nextTile);
}

void CNetworkPlayScene::CollectClosedTile(IndexedPosition indexedPosition, Direction direction)
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
				//memset(m_ClosedTile, 0, sizeof(IndexedPosition) * CHECKLIST_LENGTH);

				for (int i = 0; i < m_ClosedTile.size(); ++i)
				{
					m_ClosedTile[i].m_PosI = 0;
					m_ClosedTile[i].m_PosJ = 0;
				}
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
void CNetworkPlayScene::InitRandomMap()
{
	// 울타리, 금, 쓰레기의 초기값을 각각 설정해줍니다.
	int startLineNumber =	m_PlayerNumber * 5;
	int startGoldNumber =	m_PlayerNumber * 2;
	int startTrashNumber =	m_PlayerNumber * 2;

	// IsClosed에서 사용할 변수들입니다.
	IndexedPosition RandomTargetPosition;

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

bool CNetworkPlayScene::IsEnd()
{
	return (m_Map->GetVoidTileCount() == 0);
}


void CNetworkPlayScene::Render()
{
	// 그려야 할 것이 있으면 그린다
	if (CNetworkManager::GetInstance()->GetDrawLineFlag() )
	{
		CNetworkManager::GetInstance()->SetDrawLineFlag(false);

		IndexedPosition position = CNetworkManager::GetInstance()->GetIndexedPositionFromServer();
		int currentPlayerId = CNetworkManager::GetInstance()->GetCurrentTurnId();

		DrawLineFromServer( position, currentPlayerId );

	}

	for (auto iter: m_Object)
	{
		iter->Render();
	}

	CNetworkGameTimer::GetInstance()->Update();
	//timer 여기에 추가할 것
	CNetworkGameTimer::GetInstance()->Render();

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

void CNetworkPlayScene::PlayBGM()
{
	CSoundRenderer::GetInstance()->PlayBGM(BGM_PLAY);
}

void CNetworkPlayScene::StopBGM()
{
	CSoundRenderer::GetInstance()->StopBGM();
}

void CNetworkPlayScene::ResizeClient()
{
	for (auto iter: m_Object)
	{
		iter->ResizeClient();
	}

	SetClickArea();
}

void CNetworkPlayScene::DrawLineFromServer(const IndexedPosition& indexedPosition, int clientId)
{
	CSoundRenderer::GetInstance()->PlaySE_DrawLine();

	assert(indexedPosition.m_PosI < MAX_MAP_HEIGHT && indexedPosition.m_PosI > 0);
	assert(indexedPosition.m_PosJ < MAX_MAP_HEIGHT && indexedPosition.m_PosJ > 0);

	m_Map->DrawLine(indexedPosition);

	for (int i = 0; i < m_ClosedTile.size(); ++i)
	{
		m_ClosedTile[i].m_PosI = 0;
		m_ClosedTile[i].m_PosJ = 0;
	}

	if (IsClosed(indexedPosition))
	{
		int i = 0;
		while (m_ClosedTile[i].m_PosI != 0 && m_ClosedTile[i].m_PosJ != 0 )
		{
			//본래 타일에 뭐가 있었는지 확인해서 각자 바꿀 것!!
			m_Map->SetMapOwner(m_ClosedTile[i], (MO_OWNER)(clientId) );
			m_Map->SubtractVoidCount();
			++i;
		}

		//tile animation 시작 설정
		m_Map->SetTileAnimationTurn(1);

		//많이 먹었다.
		if ( i >= 5 )
		{
			CGameData::GetInstance()->GetNetworkClinent(clientId)->PlayMonsterSound();
			//CSoundRenderer::GetInstance()->PlaySE_Player(CNetworkManager::GetInstance()->GetCurrentTurnId());
		}

#ifdef _DEBUG
		printf("우와! 플레이어 %d가 땅을 먹었다!\n",(clientId));
#endif
	}

	if (IsEnd() )
	{
		m_EndFlag = true;

		GameEndRequest sendData;
		sendData.mClientId = CNetworkManager::GetInstance()->GetClientId();

		if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&sendData, sendData.mSize) )
		{
			CNetworkManager::GetInstance()->PostSendMessage();
		}
	}

}