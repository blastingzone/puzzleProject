#include "stdafx.h"
#include "NetworkPlayScene.h"
#include "GameTimer.h"
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

	m_Map = new CGameMap(CGameData::GetInstance()->GetMapSize());

	if ( m_Map == nullptr || !m_Map->Init() )
	{
		return false;
	}

	LinkPlayers();

	InitRandomMap();
	AddObject(m_Map);

	SetClickArea();
	CGameTimer::GetInstance()->SetTimerStart();

	PlayBGM();

	return true;
}

void CNetworkPlayScene::SetClickArea()
{
	m_ClickBuffer = SC_P_CLICK_BUFFER * CRenderer::GetInstance()->GetDisplayScale();
	m_ClickLineWeight = m_Map->GetLineWeight() + (m_ClickBuffer * 2);
	m_ClickTileSize = m_Map->GetTileSize() - (m_ClickBuffer * 2);
}

//���� ���� ������ ������Ʈ ���ִ� �Լ�
void CNetworkPlayScene::EventHandle(Coordinate mouseCoordinate)
{
	//�Էµ� ���콺 ������ ��ġ�� ���� �� ���� ���̰� �ִϸ��̼� ��� ���� �ƴ� ���� ó��
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
	if (IsPossible(indexedPosition) )
	{
		CSoundRenderer::GetInstance()->PlaySE_DrawLine();
#ifdef _DEBUG
		printf("<<< ---- ���� �÷��̾� : %d ---- >>>\n",(m_PlayerTurn%m_PlayerNumber));
		printf(" i : %d, j : %d\n",indexedPosition.m_PosI,indexedPosition.m_PosJ);
#endif
		//IsPossible üũ �Ŀ� gameMap ȣ���ؼ� �ݿ�
		m_Map->DrawLine(indexedPosition);
		//memset(m_ClosedTile, 0, sizeof(IndexedPosition) * CHECKLIST_LENGTH);

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
				//���� Ÿ�Ͽ� ���� �־����� Ȯ���ؼ� ���� �ٲ� ��!!
				m_Map->SetMapOwner(m_ClosedTile[i], (MO_OWNER)(m_PlayerTurn % m_PlayerNumber) );
				m_Map->SubtractVoidCount();
				i++;
			}

			//tile animation ���� ����
			m_Map->SetTileAnimationTurn(1);
#ifdef _DEBUG
			printf("���! �÷��̾� %d�� ���� �Ծ���!\n",(m_PlayerTurn%m_PlayerNumber));
#endif
		}

		if (IsEnd() )
		{
			m_Map->WriteResult();
			CGameData::GetInstance()->SetCurrentScene( SC_RESULT );
		}

		++m_PlayerTurn;

		m_Map->SetCurrentTurn(m_PlayerTurn%m_PlayerNumber);

		CGameTimer::GetInstance()->SetTimerStart();
	}
}

void CNetworkPlayScene::MouseOver(Coordinate mouseCoordinate)
{
	m_Map->InitVirtualLineState();

	//�Էµ� ���콺 ������ ��ġ�� ���� �� ���� ���� ���� ó��
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
		// ��Ÿ���� (2,1), (1,2) ���� �����ϹǷ�
		RandomTargetPosition.m_PosI = rand() % MAX_MAP_HEIGHT + 1; 
		RandomTargetPosition.m_PosJ = rand() % MAX_MAP_WIDTH + 1;

		// ���� ������ ���� ��ǥ�� MO_LINE_UNCONNECTED�� ��쿡
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

//���콺 ��ǥ���� index�� �ٲٴ� �Լ�
IndexedPosition CNetworkPlayScene::CalculateIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;

	//���콺�� ��ġ�� ���� �׷����� ������ ��ǥ�踦 �������� ��ȯ
	mouseCoordinate.m_PosX -= static_cast<int>(m_Map->GetStartPosition().width) - m_ClickBuffer;
	mouseCoordinate.m_PosY -= static_cast<int>(m_Map->GetStartPosition().height) - m_ClickBuffer;

	float indexUnit = m_ClickLineWeight + m_ClickTileSize;
	//Ÿ�� �ϳ��� ���� �ϳ��� ��� ��ⷯ �������� �ε��� �� ���
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
	//SettingScene������ �÷��̾� ���� �޾ƿ´�.
	m_PlayerNumber = CGameData::GetInstance()->GetplayerNumber();
}

void CNetworkPlayScene::LinkPlayers()
{
	std::array<int, MAX_PLAYER_NUM> PlayerTurn = {0, 1, 2, 3};

	srand( static_cast<unsigned int>(time(NULL)) );
	std::random_shuffle(PlayerTurn.begin(), PlayerTurn.end());

	//player turn ����
	int joinPlayerIdx = 0;
	int notJoinPlayerIdx = m_PlayerNumber;
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		//���� ���ӿ� �����ϴ� �÷��̾�(ĳ����)��� �÷��̾� ���� �����ϴ� ���̺� ���ʿ� �߰�
		//m_map�� �ߺ� ������
		if (CGameData::GetInstance()->GetPlayerCreatedFlag(PlayerTurn[i]) )
		{
			assert(joinPlayerIdx >= 0 && joinPlayerIdx < MAX_PLAYER_NUM);

			m_Player[joinPlayerIdx] = CGameData::GetInstance()->GetPlayerPtr(PlayerTurn[i]);
			m_Map->SetPlayerTurnTable(joinPlayerIdx, CGameData::GetInstance()->GetPlayerPtr(PlayerTurn[i]) );

			//gamedata�� player turn ����
			CGameData::GetInstance()->SetPlayerTurn(PlayerTurn[i], joinPlayerIdx++);
		}
		else
		{
			assert(notJoinPlayerIdx >= 0 && notJoinPlayerIdx < MAX_PLAYER_NUM);

			m_Player[notJoinPlayerIdx] = CGameData::GetInstance()->GetPlayerPtr(PlayerTurn[i]);
			m_Map->SetPlayerTurnTable(notJoinPlayerIdx, CGameData::GetInstance()->GetPlayerPtr(PlayerTurn[i]) );

			//gamedata�� player turn ����
			CGameData::GetInstance()->SetPlayerTurn(PlayerTurn[i], notJoinPlayerIdx++);
		}
	}
}

bool CNetworkPlayScene::IsClosed( IndexedPosition indexedPosition)
{

#ifdef _DEBUG
	printf("idx I : %d / idx J : %d\n", indexedPosition.m_PosI, indexedPosition.m_PosJ);
#endif

	//���õ� ��Ÿ���� ���� Ȯ��
	CollectClosedTile(indexedPosition,DI_UP);
	if (m_ClosedTile[0].m_PosI != 0 && m_ClosedTile[0].m_PosJ != 0)
		return true;

	//���õ� ��Ÿ���� ������ Ȯ��
	CollectClosedTile(indexedPosition, DI_RIGHT);
	if (m_ClosedTile[0].m_PosI != 0 && m_ClosedTile[0].m_PosJ != 0)
		return true;

	//���õ� ��Ÿ���� �Ʒ��� Ȯ��
	CollectClosedTile(indexedPosition, DI_DOWN);
	if (m_ClosedTile[0].m_PosI != 0 && m_ClosedTile[0].m_PosJ != 0)
		return true;

	//���õ� ��Ÿ���� ���� Ȯ��
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

		//�Էµ� ��Ÿ�� �ֺ��� Ȯ���ؼ� �����ְ� ���� Ÿ�ϰ� ��Ƽ���� ���ڸ� ����
		if (m_Map->GetMapOwner(indexedPosition.m_PosI + 1, indexedPosition.m_PosJ) == MO_NOBODY) { ++tileVoidCount; }

		if (m_Map->GetMapOwner(indexedPosition.m_PosI - 1, indexedPosition.m_PosJ) == MO_NOBODY) { ++tileVoidCount; }

		if (m_Map->GetMapOwner(indexedPosition.m_PosI, indexedPosition.m_PosJ + 1) == MO_NOBODY) { ++tileVoidCount; }

		if (m_Map->GetMapOwner(indexedPosition.m_PosI, indexedPosition.m_PosJ - 1) == MO_NOBODY) { ++tileVoidCount; }

		//Ȯ�ε� Ÿ���� ���� 4�� �Ǹ� �Էµ� ��Ÿ���� ���� Ÿ�ϵ� ���̿� �����Ƿ� ���� �� ����??
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

	//Ȯ�� �� ������ ����
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

	//Ȯ�� �� ������ ������� ���̸� Ȯ�� �� ��
	if (m_Map->GetMapType(currentTile) != MO_DOT)
	{
		//Ÿ�Ͽ� �ִϸ��̼� �����ϴ� ���� ��Ÿ���� ���� ����
		int animationTurn = 1;
		m_Map->SetAnimationState(currentTile, animationTurn, direction);

		//�տ��� ������ Ž�� ��� ������ ť�� �迭�� �ִ´�.
		int i = 0;

		searchTiles.push(currentTile);
		m_ClosedTile[i++] = currentTile;
		m_Map->SetMapFlag(currentTile, true);

		while (!searchTiles.empty() )
		{
			currentTile.m_PosI = searchTiles.front().m_PosI;
			currentTile.m_PosJ = searchTiles.front().m_PosJ;
			searchTiles.pop();

			//currentTile�� sentinel�̸� ���ݱ��� Ȯ���� �������δ� ������ ���������Ƿ� Ȯ���� Ÿ���� �����ϴ� �迭�� �ʱ�ȭ�ϰ� Ȯ�� ����
			if (m_Map->GetMapType(currentTile) == MO_SENTINEL)
			{
				for (int tempI = 0 ; tempI < MAX_MAP_WIDTH; ++tempI)
				{
					for (int tempJ = 0 ; tempJ < MAX_MAP_HEIGHT; ++tempJ)
					{
						m_Map->SetMapFlag(IndexedPosition(tempI, tempJ), false);

						if (m_Map->GetMapType(IndexedPosition(tempI, tempJ) ) == MO_TILE)
						{
							//�ִϸ��̼� ����� ���� �����͵� �ʱ�ȭ
							m_Map->InitAnimationState(IndexedPosition(tempI, tempJ) );
						}

						//����� �ִϸ��̼��� �����Ƿ� 0���� ����
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
				printf("��Ƽ���� �������\n");
#endif
				break;
			}
#ifdef _DEBUG
			printf("idx I : %d / idx J : %d\n", currentTile.m_PosI, currentTile.m_PosJ);
#endif
			//���� Ÿ���� ���� Ȯ��
			if (m_Map->GetMapType(currentTile.m_PosI - 1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{
				nextTile.m_PosI = currentTile.m_PosI - 2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(nextTile) )
				{
					searchTiles.push(nextTile);
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);

					//�ִϸ��̼� ����� ���� ������ ���� ����
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_UP);
				}				
			}

			//���� Ÿ���� ������ Ȯ��
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ + 1) == MO_LINE_UNCONNECTED)
			{
				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ + 2;
				if (!IsAlreadyChecked(nextTile) )
				{
					searchTiles.push(nextTile);
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);

					//�ִϸ��̼� ����� ���� ������ ���� ����
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_RIGHT);
				}				
			}

			//���� Ÿ���� �Ʒ��� Ȯ��
			if (m_Map->GetMapType(currentTile.m_PosI + 1, currentTile.m_PosJ) == MO_LINE_UNCONNECTED)
			{
				nextTile.m_PosI = currentTile.m_PosI + 2;
				nextTile.m_PosJ = currentTile.m_PosJ;
				if (!IsAlreadyChecked(nextTile) )
				{
					searchTiles.push(nextTile);
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);

					//�ִϸ��̼� ����� ���� ������ ���� ����
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_DOWN);
				}				
			}

			//���� Ÿ���� ���� Ȯ��
			if (m_Map->GetMapType(currentTile.m_PosI, currentTile.m_PosJ - 1) == MO_LINE_UNCONNECTED)
			{
				nextTile.m_PosI = currentTile.m_PosI;
				nextTile.m_PosJ = currentTile.m_PosJ - 2;
				if (!IsAlreadyChecked(nextTile) )
				{
					searchTiles.push(nextTile);
					m_ClosedTile[i++] = nextTile;
					m_Map->SetMapFlag(nextTile, true);

					//�ִϸ��̼� ����� ���� ������ ���� ����
					animationTurn = m_Map->GetTileAnimationTurn(currentTile) + 1;
					m_Map->SetAnimationState(nextTile, animationTurn, DI_LEFT);
				}				
			}
		}
		//���� Ÿ���� �����Ƿ� �ִϸ��̼� �����϶�� �÷��� ����
		m_Map->SetTileAnimationTurnNumber(animationTurn);
	}
}

// �ʱ⿡ �ʿ� �������� �����۰� ��Ÿ���� ��ġ�ϴ� �Լ��Դϴ�.
void CNetworkPlayScene::InitRandomMap()
{
	// ��Ÿ��, ��, �������� �ʱⰪ�� ���� �������ݴϴ�.
	int startLineNumber =	m_PlayerNumber * 5;
	int startGoldNumber =	m_PlayerNumber * 2;
	int startTrashNumber =	m_PlayerNumber * 2;

	// IsClosed���� ����� �������Դϴ�.
	IndexedPosition RandomTargetPosition;

	// �������� �õ尪�� �ʿ��� ��� �Ʒ� �õ带 �Ẹ�ñ� �ٶ��ϴ�.
	//srand(1383706550);

	// ��Ÿ���� �ѷ��ִ� �Լ��Դϴ�.
	while (startLineNumber)
	{
		// ��Ÿ���� (2,1), (1,2) ���� �����ϹǷ�
		RandomTargetPosition.m_PosI = rand() % MAX_MAP_HEIGHT + 1; 
		RandomTargetPosition.m_PosJ = rand() % MAX_MAP_WIDTH + 1;

		// ���� ������ ���� ��ǥ�� MO_LINE_UNCONNECTED�� ��쿡
		if ( m_Map->GetMapType(RandomTargetPosition) == MO_LINE_UNCONNECTED )
		{
			// IsPossible�� �����ϸ�
			if ( IsPossible(RandomTargetPosition) )
			{
				// �ϴ� �׸��ϴ�(IsClosed �˻縦 ���ؼ�)
				//printf("random %d , %d\n",RandomTargetPosition.m_PosI,RandomTargetPosition.m_PosJ);
				m_Map->DrawLine(RandomTargetPosition);
				--startLineNumber;

				// ���� �� �׷��� ���� IsClosed() ������ �������� ������ �״�� ����
				if ( IsClosed(RandomTargetPosition) )
				{
					// ���� ���� �� �׷��� ���� � ������ �ݴ´ٸ� �ش� ���� �����ϰ� ���� ī���͸� ����
					m_Map->DeleteLine(RandomTargetPosition);
					++startLineNumber;
				}
			}
		}

	}

	// Ÿ�� �Ӽ��� ��׵�� �ٲٱ� ����
	// 1. �귯���� ���� �غ��ؼ� Ÿ�� ���� �׷���� �մϴ�
	// 2. �׸��� IsClosed ���� �� ���ư����� �� �� �׽�Ʈ �� ���� ��¥ �ݿ�
	// 3. ��׵��� �̹��� ���Ϸ� �־��ִ°� ���� �� �մϴ�

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
	for (auto iter: m_Object)
	{
		iter->Render();
	}

	CGameTimer::GetInstance()->Update();
	//timer ���⿡ �߰��� ��
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