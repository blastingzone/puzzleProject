#include "stdafx.h"
#include "NetworkGameMap.h"
#include "NetworkGameTimer.h"
#include "Player.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "PacketType.h"
#include "NetworkManager.h"

CNetworkGameMap::CNetworkGameMap(MapSize mapSize)
{
	m_pRenderTarget = nullptr;

	m_pDotBrush = nullptr;
	m_pUnconnectedLineBrush = nullptr;
	m_pConnectedLineBrush = nullptr;
	m_pPossibleLineBrush = nullptr;
	m_pTileBrush = nullptr;
	m_pVoidTileBrush = nullptr;

	m_pTimer = nullptr;

	m_LineAnimationFlag = false;
	m_TileAnimationTurnNumber = 0;
	m_TileAnimationTurn = 0;
	m_TileAnimationTurnOver = false;

	m_TileSize = 0;
	m_LineWeight = 0;
	m_DotRadius = 0;
	m_ItemRadius = 0;
	m_ProfileWidth = 0;
	m_ProfileHeight = 0;

	m_TimerPositionHeight = 0;
	m_TimerWidth = 0;
	m_TimerHeight = 0;

	m_MapSize.m_Width = mapSize.m_Width;
	m_MapSize.m_Height = mapSize.m_Height;

	m_VoidTileCount = m_MapSize.m_Width * m_MapSize.m_Height;

	m_isMouseOn = false;

	m_CurrentTurn = 0;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		m_CharacterByClientId[i] = nullptr;
		m_PlayerAnimation[i] = nullptr;
	}

	m_TurnPointer[0] = nullptr;
	m_TurnPointer[1] = nullptr;

	m_DWriteFactory = nullptr;
	m_PlayerNameTextFormat = nullptr;
	m_pPlayerNameTextBrush = nullptr;
	
	m_PlayerNameTextSize = 0;
	m_PlayerNameTextMaginV = 0;
	m_PlayerNameTextMaginH = 0;
}

CNetworkGameMap::~CNetworkGameMap(void)
{
	SafeRelease(m_pDotBrush);
	SafeRelease(m_pUnconnectedLineBrush);
	SafeRelease(m_pConnectedLineBrush);
	SafeRelease(m_pPossibleLineBrush);
	SafeRelease(m_pTileBrush);
	SafeRelease(m_pVoidTileBrush);
	SafeRelease(m_DWriteFactory);
	SafeRelease(m_PlayerNameTextFormat);
	SafeRelease(m_pPlayerNameTextBrush);

	SafeRelease(m_pTimer);
}


void CNetworkGameMap::CreateMap()
{
	/*	실제로 게임에 사용되는 타일 외에도 울타리와 점을 표시하기 위한 칸도 필요
	생성된 게임 주변은 기본값인 MO_SENTINEL로 두어서 IsClosed()와 같은 작업시 활용할 수 있도록 함 */
	int targetRow, targetColumn;

	for (targetRow = 1; targetRow <= m_MapSize.m_Height*2 + 1; ++targetRow)
	{  		
		for (targetColumn = 1; targetColumn <= m_MapSize.m_Width*2 + 1; ++targetColumn) 
		{ 
			if (targetRow % 2 == 1)
			{
				// dot - line - dot - line
				if (targetColumn % 2 == 1)
				{
					m_Map[targetRow][targetColumn].m_Type = MO_DOT;
				} 
				else 
				{
					m_Map[targetRow][targetColumn].m_Type = MO_LINE_UNCONNECTED;
				}
			} 
			else 
			{
				// line - tile - line - tile
				if (targetColumn % 2 == 1)
				{
					m_Map[targetRow][targetColumn].m_Type = MO_LINE_UNCONNECTED;
				} 
				else 
				{
					m_Map[targetRow][targetColumn].m_Type = MO_TILE;
				}
			}
		}
	}

	return;
}

bool CNetworkGameMap::Init()
{
	if (!CreateResource() )
	{
		return false;
	}

	ResizeClient();
	SetMapSize(m_MapSize);
	CreateMap();

	return true;
}

void CNetworkGameMap::DrawPlayerUI()
{
	D2D1_RECT_F	textPosition;

	GetPlayerUIPosition();

	for ( int i = 0; i<CNetworkManager::GetInstance()->GetPlayerNumber();i++)
	{
		if (m_PlayerAnimation[i] == nullptr)
		{
			m_PlayerAnimation[i] = new CAnimationRenderer(m_CharacterByClientId[i]->GetFaceAnimation());
			m_PlayerAnimation[i]->LoadAnimationImage(190.0f,230.f,0.3f,S_LT_INFINITE);
		}
	}


	int position = 0;

	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		if ( m_CharacterByClientId[i] != nullptr )
		{
			if (CNetworkManager::GetInstance()->GetCurrentTurnId() == i)
			{
				m_PlayerAnimation[i]->StartAnimation(m_ProfilePosition[position]);
				m_TurnPointer[i%2]->StartAnimation(m_ProfilePosition[position]);
			}
			else
			{
				m_PlayerAnimation[i]->PauseAnimation();
				m_PlayerAnimation[i]->ShowAnimationFristFrame(m_ProfilePosition[position]);
			}
			//m_pRenderTarget -> DrawBitmap(m_CharacterByClientId[i]->GetPlayerFace(), m_ProfilePosition[position]);
			
			//이름 표현할 것
			textPosition = D2D1::Rect(
				m_ProfilePosition[i].left, 
				m_ProfilePosition[i].bottom - m_PlayerNameTextSize - m_PlayerNameTextMaginV, 
				m_ProfilePosition[i].right, 
				m_ProfilePosition[i].bottom - m_PlayerNameTextMaginV
				);

			m_pRenderTarget->DrawText(
				m_CharacterByClientId[i]->GetPlayerName().c_str(),
				m_CharacterByClientId[i]->GetPlayerName().length(),
				m_PlayerNameTextFormat,
				textPosition,
				m_pPlayerNameTextBrush
				);

			/*
			if (i == CNetworkManager::GetInstance()->GetCurrentTurnId() )
			{
			m_pRenderTarget -> DrawBitmap(m_CharacterByClientId[i]->GetPlayerBox(), m_ProfileBoxPosition[position]);
			}
			else
			{
			m_pRenderTarget -> DrawBitmap(m_CharacterByClientId[i]->GetPlayerWaitingBox(), m_ProfileBoxPosition[position]);
			}
			*/
			++position;
		}
	}
}


void CNetworkGameMap::Render()
{
	D2D1_ELLIPSE	m_DotEllipse;
	D2D1_RECT_F		rectElement;
	D2D1_POINT_2F	m_pos;

	/*	애니메이션 추가
	기본적으로 애니메이션 flag는 line과 tile 두 가지가 있음
	이 중에서 하나라도 진행 중(flag가 true)이면 마우스 입력은 받지 않고 애니메이션이 완료 되어야 게임 진행
	애니메이션 순서는 line >> tile
	타일 애니메이션에는 재생되는 순서가 지정되어 있음
	(애니메이션이 지정된 타일들의 재생이 모두 완료되어야 전체 타일 애니메이션 종료) */

	/*	layer : background - tile - line - dot 순서대로 렌더링 
	현재는 겹쳐져서 표시되는 것이 없으므로 필요없음
	추후 아이템과 UI 추가 시 상황에 맞춰서 레이어 구분 새로 할 것 */

	//background
	m_pRenderTarget->DrawBitmap(CGameData::GetInstance()->GetBackgroundImage(), D2D1::RectF(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT) );

	DrawPlayerUI();

	/*	tile layer */
	ID2D1SolidColorBrush* tempTileBrush = nullptr;

	for (int i = 0; i <= MAX_MAP_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_MAP_HEIGHT; ++j)
		{
			if (GetMapType(IndexedPosition(i, j) ) == MO_TILE)
			{
				m_pos.y = m_StartPosition.height + ( (m_LineWeight + m_TileSize) / 2 ) * (i -1) + m_LineWeight / 2;
				m_pos.x = m_StartPosition.width + ( (m_LineWeight + m_TileSize) / 2 ) * (j - 1) + m_LineWeight / 2;
				rectElement = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_TileSize / 2, m_pos.x + m_TileSize / 2, m_pos.y + m_TileSize / 2);

				//일단 이번 타일을 칠할 브러시를 할당하고
				switch (GetMapOwner(IndexedPosition(i, j) ) )
				{
				case MO_NOBODY:
					tempTileBrush = m_pVoidTileBrush;
					break;
				case MO_PLAYER1:
					tempTileBrush = m_CharacterByClientId[0]->GetPlayerBrush();
					break;
				case MO_PLAYER2:
					tempTileBrush = m_CharacterByClientId[1]->GetPlayerBrush();
					break;
				case MO_PLAYER3:
					tempTileBrush = m_CharacterByClientId[2]->GetPlayerBrush();
					break;
				case MO_PLAYER4:
					tempTileBrush = m_CharacterByClientId[3]->GetPlayerBrush();
					break;
				default:
					break;
				}


				//그릴지 기다릴지 결정(애니메이션 순서에 따라서)
				if (!m_Map[i][j].m_AnimationFlag)
				{
					m_pRenderTarget->FillRectangle(rectElement, tempTileBrush);
				}
				else
				{
					//아직 애니메이션을 그릴 차례가 아니면 소유주가 없는 상태로 덮어 버린다.
					if ( (m_Map[i][j].m_AnimationTurn > m_TileAnimationTurn && m_TileAnimationTurn != 0)
						|| (m_Map[i][j].m_AnimationTurn != 0 && m_LineAnimationFlag) )
					{
						//m_pRenderTarget->FillRectangle(rectElement, m_pVoidTileBrush);
					}
					//진행중인 라인 애니메이션이 없고 
					//애니메이션 그릴 턴이 되었거나 진행 중이라면
					else if (!m_LineAnimationFlag 
						&& m_Map[i][j].m_AnimationTurn <= m_TileAnimationTurn
						&& m_TileAnimationTurn > 0
						&& m_Map[i][j].m_AnimationTurn > 0)
					{
						if (m_Map[i][j].m_StartTime == 0)
						{
							m_Map[i][j].m_StartTime = CNetworkGameTimer::GetInstance()->GetTime();
						}

						DWORD currentTime = CNetworkGameTimer::GetInstance()->GetTime();

						float progressedTimeRatio = (currentTime - m_Map[i][j].m_StartTime) / static_cast<float>(SC_P_TILE_ANIMATION_TIME);

						//만약 애니메이션 재생 시간을 초과하면 최대 크기로 할당
						float tempLine = 0;
						if (progressedTimeRatio >= 1)
						{
							tempLine = m_TileSize;
						}
						else
						{
							tempLine = m_TileSize * sin(progressedTimeRatio * 3.14f / 2);
						}

						if ( tempLine >= m_TileSize)
						{
							//애니메이션 재생 완료
							m_TileAnimationTurnOver = true;

							m_Map[i][j].m_AnimationTurn = 0;
							m_Map[i][j].m_AnimationFlag = false;
						}
						else
						{
							//애니메이션이 재생 중이라면 
							D2D1_RECT_F animationRect;

							//애니메이션을 그린다
							switch (m_Map[i][j].m_Direction)
							{
							case DI_UP:
								animationRect = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y + m_TileSize / 2 - tempLine, m_pos.x + m_TileSize / 2, m_pos.y + m_TileSize / 2);
								break;
							case DI_RIGHT:
								animationRect = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_TileSize / 2, m_pos.x - m_TileSize / 2 + tempLine, m_pos.y + m_TileSize / 2);
								break;
							case DI_DOWN:
								animationRect = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_TileSize / 2, m_pos.x + m_TileSize / 2, m_pos.y - m_TileSize / 2 + tempLine);
								break;
							case DI_LEFT:
								animationRect = D2D1::Rect( m_pos.x + m_TileSize / 2 - tempLine, m_pos.y - m_TileSize / 2, m_pos.x + m_TileSize / 2, m_pos.y + m_TileSize / 2);
								break;
							default:
								break;
							}

							m_pRenderTarget->FillRectangle(animationRect, tempTileBrush);
						}
					}
				}

				//item draw
				if (GetItem(IndexedPosition(i, j) ) != MO_NOTHING)
				{
					m_DotEllipse = D2D1::Ellipse( m_pos, m_ItemRadius, m_ItemRadius );
					switch (GetItem(IndexedPosition(i, j) ) )
					{
					case MO_GOLD:
						//m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pGoldBrush);
						m_pRenderTarget->DrawBitmap(m_gold,rectElement);
						break;
					case MO_TRASH:
						//m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pTrashBrush);
						m_pRenderTarget->DrawBitmap(m_trash,rectElement);
						break;
					default:
						break;
					}

				}
			}
		}
	}
	if (m_TileAnimationTurnOver)
	{
		m_TileAnimationTurnOver = false;

		//애니메이션 재생이 완료되면 현재 타일 변수 초기화 및 다음 턴 그릴 준비
		//m_TileAnimationTurn = m_Map[i][j].m_AnimationTurn + 1;
		++m_TileAnimationTurn;
						
		//혹시라도 애니메이션 재생 시간이 20초를 넘길 경우를 대비해서 주기적으로 초기화
		//CNetworkGameTimer::GetInstance()->SetTimerStart();
						
		//타일 애니메이션 전체가 끝난거라면 관련 변수 초기화
		if (m_TileAnimationTurn > m_TileAnimationTurnNumber)
		{
			m_TileAnimationTurn = 0;
			m_TileAnimationTurnNumber = 0;

			// 서버에 Ready 상태 보고
			TurnReadyRequest sendData;
			sendData.mClientId = CNetworkManager::GetInstance()->GetClientId();

			if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&sendData, sendData.mSize) )
			{
				CNetworkManager::GetInstance()->PostSendMessage();
			}
		}
	}

	/*	line layer */
	for (int i = 0; i <= MAX_MAP_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_MAP_HEIGHT; ++j)
		{
			if ( GetMapType(IndexedPosition(i, j) ) == MO_LINE_UNCONNECTED || GetMapType(IndexedPosition(i, j) ) == MO_LINE_CONNECTED || GetMapType(IndexedPosition(i, j) ) == MO_LINE_HIDDEN)
			{
				if (i%2==0)
				{
					m_pos.y = m_StartPosition.height + ( (m_LineWeight + m_TileSize) / 2 ) * (i - 1) + m_LineWeight / 2;
					m_pos.x = m_StartPosition.width + ( (m_LineWeight + m_TileSize) / 2 ) * (j - 1) + m_LineWeight / 2;
					rectElement = D2D1::Rect( m_pos.x - m_LineWeight / 2, m_pos.y - m_TileSize / 2, m_pos.x + m_LineWeight / 2, m_pos.y + m_TileSize / 2);
				}
				else
				{
					m_pos.y = m_StartPosition.height + ( (m_LineWeight + m_TileSize) / 2 ) * (i - 1) + m_LineWeight / 2;
					m_pos.x = m_StartPosition.width + ( (m_LineWeight + m_TileSize) / 2 ) * (j - 1) + m_LineWeight / 2;
					rectElement = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_LineWeight / 2, m_pos.x + m_TileSize / 2, m_pos.y + m_LineWeight / 2);
				}
				switch (GetMapType(IndexedPosition(i, j)) )
				{
				case MO_LINE_UNCONNECTED:
					m_pRenderTarget->FillRectangle(rectElement, m_pUnconnectedLineBrush);
					break;
				case MO_LINE_CONNECTED:
					//animation
					//GET - SET function 만들 것
					if (m_Map[i][j].m_AnimationFlag)
					{
						//UnconnectedLine을 배경에 두고 위에 그려야 하므로 배경을 먼저 그림
						m_pRenderTarget->FillRectangle(rectElement, m_pUnconnectedLineBrush);

						if (m_Map[i][j].m_StartTime == 0)
						{
							m_Map[i][j].m_StartTime = CNetworkGameTimer::GetInstance()->GetTime();
						}

						DWORD currentTime = CNetworkGameTimer::GetInstance()->GetTime();
						float timeWeight = currentTime - m_Map[i][j].m_StartTime;

						float tempLine = m_TileSize * ( timeWeight / SC_P_LINE_ANIMATION_TIME);

						if (tempLine > m_TileSize)
						{
							//line 애니메이션 종료되면 관련 변수 초기화
							m_Map[i][j].m_AnimationFlag = false;
							m_LineAnimationFlag = false;
							m_pRenderTarget->FillRectangle(rectElement, m_pConnectedLineBrush);

							//다음에 재생할 타일 애니메이션이 없으면 서버에 보고
							if (m_TileAnimationTurnNumber == 0)
							{
								// 서버에 Ready 상태 보고
								TurnReadyRequest sendData;
								sendData.mClientId = CNetworkManager::GetInstance()->GetClientId();

								if (CNetworkManager::GetInstance()->GetSendBuffer()->Write(&sendData, sendData.mSize) )
								{
									CNetworkManager::GetInstance()->PostSendMessage();
								}
							}
						}
						else
						{
							//애니메이션 재생
							if (i%2==0)
							{
								rectElement = D2D1::Rect( m_pos.x - m_LineWeight / 2, m_pos.y - tempLine / 2, m_pos.x + m_LineWeight / 2, m_pos.y + tempLine / 2);
							}
							else
							{
								rectElement = D2D1::Rect( m_pos.x - tempLine / 2, m_pos.y - m_LineWeight / 2, m_pos.x + tempLine / 2, m_pos.y + m_LineWeight / 2);
							}
							m_pRenderTarget->FillRectangle(rectElement, m_pConnectedLineBrush);
						}
					}
					else
					{
						m_pRenderTarget->FillRectangle(rectElement, m_pConnectedLineBrush);
					}
					break;
				default:
					break;
				}
				if (m_Map[i][j].m_MouseOverFlag == true)
				{
					m_pRenderTarget->FillRectangle(rectElement, m_pPossibleLineBrush);
				}
			}
		}
	}

	/*	dot layer */
	for (int i = 0; i <= MAX_MAP_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_MAP_HEIGHT; ++j)
		{
			if (GetMapType(IndexedPosition(i, j) ) == MO_DOT)
			{
				m_pos.x = m_StartPosition.width + ( (m_LineWeight + m_TileSize) / 2 ) * (j - 1) + m_LineWeight / 2;
				m_pos.y = m_StartPosition.height + ( (m_LineWeight + m_TileSize) / 2 ) * (i - 1) + m_LineWeight / 2;

				m_DotEllipse = D2D1::Ellipse( m_pos, m_DotRadius, m_DotRadius );
				m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pDotBrush);
			}
		}
	}

	D2D1_SIZE_F centerPosition;
	centerPosition = m_pRenderTarget->GetSize();

	centerPosition.width /= 2;

	m_pos.x = centerPosition.width;
	m_pos.y = m_TimerPositionHeight;

	//배경
	rectElement = D2D1::Rect(m_StartPosition.width, m_pos.y, m_StartPosition.width + m_TimerWidth, m_pos.y + m_TimerHeight);
	m_pRenderTarget->FillRectangle(rectElement, m_pUnconnectedLineBrush);

	float remainTimeRatio = (CNetworkGameTimer::GetInstance()->GetRemainTime() / static_cast<float>(TIME_LIMIT) );

	//animation 재생 중에는 타이머 안 줄어드는 것처럼 보이게 함
	if (m_LineAnimationFlag || m_TileAnimationTurn != 0)
	{
		remainTimeRatio = 1.0f;
	}

	float currentTimerLength = m_TimerWidth * remainTimeRatio;

	//남은 시간 표시 (브러시 새로 만들어 쓸 것)
	rectElement = D2D1::Rect(m_StartPosition.width, m_pos.y, m_StartPosition.width + currentTimerLength, m_pos.y + m_TimerHeight);
	m_pRenderTarget->FillRectangle(rectElement, m_pTimer);
}

void CNetworkGameMap::SetMapSize(MapSize mapsize)
{
	m_MapSize.m_Width = mapsize.m_Width;
	m_MapSize.m_Height = mapsize.m_Height;
}

bool CNetworkGameMap::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(50.0f/255, 50.0f/255, 50.0f/255), &m_pDotBrush); //

		if (!SUCCEEDED(hr) )
			ErrorHandling();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(214.0f/255, 184.0f/255, 150.0f/255), &m_pUnconnectedLineBrush); //

		if (!SUCCEEDED(hr) )
			ErrorHandling();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(165.0f/255, 130.0f/255, 93.0f/255), &m_pPossibleLineBrush); //

		if (!SUCCEEDED(hr) )
			ErrorHandling();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(117.0f/255, 76.0f/255, 36.0f/255), &m_pConnectedLineBrush); //

		if (!SUCCEEDED(hr) )
			ErrorHandling();

		//조심해!! 일단 안 보이게 하고 나중에 수정
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 0.9f), &m_pVoidTileBrush);

		if (!SUCCEEDED(hr) )
			ErrorHandling();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(212.0f/255, 72.0f/255, 101.0f/255), &m_pTimer);

		if (!SUCCEEDED(hr) )
			ErrorHandling();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &m_pPlayerNameTextBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();

		hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_DWriteFactory)
            );
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();

		SetObjectSize();
		RefreshTextSize();

		m_gold = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAY_gold.png", m_gold);
		m_trash = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAY_trash.png", m_trash);

		m_TurnPointer[0] = new CAnimationRenderer(L"Resource/Image/update/PLAY_arrowbigreverse_ani.png");
		m_TurnPointer[0]->LoadAnimationImage(190.0f,230.0f,0.1f,S_LT_INFINITE);

		m_TurnPointer[1] = new CAnimationRenderer(L"Resource/Image/update/PLAY_arrowbig_ani.png");
		m_TurnPointer[1]->LoadAnimationImage(190.0f,230.0f,0.1f,S_LT_INFINITE);
	}

	return true;
}

void CNetworkGameMap::DrawLine(const IndexedPosition& indexedPosition)
{
	// 범위를 벗어난 경우 예외 처리
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_CONNECTED;

	//animaiton start
	m_LineAnimationFlag = true;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = true;
}

void CNetworkGameMap::DeleteLine( const IndexedPosition& indexedPosition )
{
	// 범위를 벗어난 경우 예외 처리
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	//랜덤 라인 긋는 과정에서의 애니메이션 상태 변화 되돌리기
	for (int tempI = 0 ; tempI < MAX_MAP_WIDTH; ++tempI)
	{
		for (int tempJ = 0 ; tempJ < MAX_MAP_HEIGHT; ++tempJ)
		{
			SetMapFlag(IndexedPosition(tempI, tempJ), false);

			if (GetMapType(IndexedPosition(tempI, tempJ) ) == MO_TILE)
			{
				//애니메이션 재생을 위한 데이터도 초기화
				InitAnimationState(IndexedPosition(tempI, tempJ) );
			}
		}
	}

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_UNCONNECTED;
}

void CNetworkGameMap::CalcStartPosition()
{
	/*	현재 화면의 중심점을 기준으로 생성된 맵의 크기의 반만큼
	왼쪽과 위쪽으로 이동한 지점을 m_StartPosition으로 지정 */
	m_CenterPosition = m_pRenderTarget->GetSize();
	m_CenterPosition.height /= 2;
	m_CenterPosition.width /= 2;

	m_StartPosition.height = 
		m_CenterPosition.height 
		- (m_MapSize.m_Height * (m_LineWeight + m_TileSize) + m_LineWeight) / 2;

	m_StartPosition.width = 
		m_CenterPosition.width 
		- (m_MapSize.m_Width * (m_LineWeight + m_TileSize) + m_LineWeight) / 2;
}

void CNetworkGameMap::ResizeClient()
{
	CalcStartPosition();
	SetObjectSize();
	RefreshTextSize();
}

void CNetworkGameMap::SetObjectSize()
{
	/*	현재 렌더러에 저장된 화면 스케일에 맞춰서 
	렌더링 할 때 사용된 오브젝트들 크기 조정 */
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	//맵 사이즈가 큰 경우에는 화면 스케일을 줄여주기 위해
	float scaleWeight = (m_MapSize.m_Height > 7) ? 0.8f : 1.0f;

	m_TileSize = tempScale * DEFAULT_TILE_SIZE * scaleWeight;
	m_LineWeight = tempScale * DEFAULT_LINE_WEIGHT * scaleWeight;
	m_DotRadius = tempScale * DEFAULT_DOT_RADIUS * scaleWeight;
	m_ItemRadius = tempScale * DEFAULT_ITEM_RADIUS * scaleWeight;

	m_TimerPositionHeight = tempScale * SC_P_TIMER_POSITION_HEIGHT;
	m_TimerWidth = tempScale * (m_MapSize.m_Width * (m_LineWeight + m_TileSize) + m_LineWeight);
	m_TimerHeight = tempScale * SC_P_TIMER_HEIGHT;

	m_ProfileWidth = tempScale * DEFAULT_CHARACTER_WIDTH;
	m_ProfileHeight = tempScale * DEFAULT_CHARACTER_HEIGHT;

	m_ProfileHorizontalMargin = tempScale * DEFAULT_CHARACTER_MARGIN_H;
	m_ProfileVerticalMargin = tempScale * DEFAULT_CHARACTER_MARGIN_V;

	m_PlayerNameTextSize = tempScale * DEFAULT_CHARACTER_NAME_TEXT_SIZE;
	m_PlayerNameTextMaginV = tempScale * DEFAULT_CHARACTER_NAME_MARGIN_V;
	m_PlayerNameTextMaginH = tempScale * DEFAULT_CHARACTER_NAME_MARGIN_H;
}

void CNetworkGameMap::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_PlayerNameTextFormat);

	hr = m_DWriteFactory->CreateTextFormat(
		_UI_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerNameTextSize,
		L"ko",
		&m_PlayerNameTextFormat
		);
	
	if (!SUCCEEDED(hr) )
		ErrorHandling();

	hr = m_PlayerNameTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	
	if (!SUCCEEDED(hr) )
		ErrorHandling();

	hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	
	if (!SUCCEEDED(hr) )
		ErrorHandling();
}

void CNetworkGameMap::GetPlayerUIPosition()
{
	m_ProfilePosition[0] = D2D1::RectF(
		m_CenterPosition.width - m_ProfileHorizontalMargin, 
		m_CenterPosition.height - m_ProfileVerticalMargin,
		m_CenterPosition.width - m_ProfileHorizontalMargin + m_ProfileWidth,
		m_CenterPosition.height - m_ProfileVerticalMargin + m_ProfileHeight);

	m_ProfilePosition[1] = D2D1::RectF(
		m_CenterPosition.width + m_ProfileHorizontalMargin - m_ProfileWidth, 
		m_CenterPosition.height - m_ProfileVerticalMargin,
		m_CenterPosition.width + m_ProfileHorizontalMargin,
		m_CenterPosition.height - m_ProfileVerticalMargin + m_ProfileHeight);

	m_ProfilePosition[2] = D2D1::RectF(
		m_CenterPosition.width - m_ProfileHorizontalMargin, 
		m_CenterPosition.height + m_ProfileVerticalMargin - m_ProfileHeight, 
		m_CenterPosition.width - m_ProfileHorizontalMargin + m_ProfileWidth, 
		m_CenterPosition.height + m_ProfileVerticalMargin);

	m_ProfilePosition[3] = D2D1::RectF(
		m_CenterPosition.width + m_ProfileHorizontalMargin - m_ProfileWidth, 
		m_CenterPosition.height + m_ProfileVerticalMargin - m_ProfileHeight, 
		m_CenterPosition.width + m_ProfileHorizontalMargin, 
		m_CenterPosition.height + m_ProfileVerticalMargin);
}

MO_TYPE CNetworkGameMap::GetMapType(IndexedPosition indexedPosition)
{
	if (indexedPosition.m_PosI < MAX_MAP_HEIGHT && indexedPosition.m_PosJ < MAX_MAP_WIDTH)
	{
		return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type;
	}

	/*	게임이 진행되는 맵 영역 밖은 모두 MO_SENTINEL로 간주하므로 
	배열 범위 밖을 벗어나도 모두 MO_SENTINEL로 처리	*/
	return MO_SENTINEL;
}

MO_TYPE CNetworkGameMap::GetMapType(const int& i, const int& j)
{
	return GetMapType(IndexedPosition(i, j) );
}

MO_OWNER CNetworkGameMap::GetMapOwner( IndexedPosition indexedPosition )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner;
}

MO_OWNER CNetworkGameMap::GetMapOwner( const int& i , const int& j )
{
	return GetMapOwner( IndexedPosition(i, j) );
}

void CNetworkGameMap::SetMapOwner( IndexedPosition indexedPosition, MO_OWNER owner )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner = owner;
}

void CNetworkGameMap::SetItem( IndexedPosition indexedPosition, MO_ITEM item )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Item = item;
}

void CNetworkGameMap::SetMapFlag( IndexedPosition indexedPosition, bool flag )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Flag = flag;
}

void CNetworkGameMap::WriteResult()
{
	for (int i = 2 ; i < MAX_MAP_HEIGHT; ++i)
	{
		for (int j = 2 ; j < MAX_MAP_WIDTH; ++j)
		{
			if ( GetMapType(i, j) == MO_TILE )
			{
				m_CharacterByClientId[static_cast<int>(GetMapOwner(i, j) )]->UpdatePlayerResult( GetItem( IndexedPosition(i, j) ) );
			}
		}
	}
}

void CNetworkGameMap::InitVirtualLineState()
{
	for (int i= 0;i<MAX_MAP_WIDTH;++i)
	{
		for (int j= 0;j<MAX_MAP_HEIGHT;++j)
		{
			if (m_Map[i][j].m_MouseOverFlag == true)
				m_Map[i][j].m_MouseOverFlag = false;
		}
	}
}

void CNetworkGameMap::ShowVirtualLine( const IndexedPosition& indexedPosition)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_MouseOverFlag = true;
}

void CNetworkGameMap::SetAnimationState(IndexedPosition indexedPosition, int turn, Direction direction)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = true;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn = turn;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Direction = direction;
}

void CNetworkGameMap::InitAnimationState(IndexedPosition indexedPosition)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn = 0;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = false;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Direction = DI_UP;
}

int	CNetworkGameMap::GetTileAnimationTurn(IndexedPosition indexedPosition)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn;
}

void CNetworkGameMap::SetPlayerTurnTable(int idx, CPlayer* playerPtr)
{
	assert(idx >= 0 && idx < MAX_PLAYER_NUM);
	m_CharacterByClientId[idx] = playerPtr;
}