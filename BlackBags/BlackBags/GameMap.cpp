#include "stdafx.h"
#include "GameMap.h"
#include "GameTimer.h"
#include "Player.h"

#define _USE_MATH_DEFINES
#include <cmath>

CGameMap::CGameMap(MapSize mapSize)
{
	m_pRenderTarget = nullptr;

	m_pDotBrush = nullptr;
	m_pUnconnectedLineBrush = nullptr;
	m_pConnectedLineBrush = nullptr;
	m_pPossibleLineBrush = nullptr;
	m_pTileBrush = nullptr;
	m_pVoidTileBrush = nullptr;
	m_pGoldBrush = nullptr;
	m_pTrashBrush = nullptr;

	m_pTimer = nullptr;

	m_LineAnimationFlag = false;
	m_TileAnimationTurnNumber = 0;
	m_TileAnimationTurn = 0;

	m_TileSize = 0;
	m_LineWeight = 0;
	m_DotRadius = 0;
	m_ItemRadius = 0;
	m_ProfileSize = 0;

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
		m_PlayerTurnTable[i] = nullptr;
	}
}

CGameMap::~CGameMap(void)
{
	SafeRelease(m_pDotBrush);
	SafeRelease(m_pUnconnectedLineBrush);
	SafeRelease(m_pConnectedLineBrush);
	SafeRelease(m_pPossibleLineBrush);
	SafeRelease(m_pTileBrush);
	SafeRelease(m_pVoidTileBrush);
	SafeRelease(m_pGoldBrush);
	SafeRelease(m_pTrashBrush);

	SafeRelease(m_pTimer);
	//SafeRelease(m_Sprite);

}


void CGameMap::CreateMap()
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

bool CGameMap::Init()
{
	if (!CreateResource() )
	{
		return false;
	}

	ResizeClient();
	SetMapSize(m_MapSize);
	CreateMap();

// 	m_Sprite = new CAnimationRenderer(L"Resource/Image/sample.png");
// 	if (m_Sprite->LoadAnimationImage(118.3f,118.3f,0.3f,S_LT_INFINITE) != true )
// 		return false;

	return true;
}

void CGameMap::DrawPlayerUI( int playerNumber )
{
	GetPlayerUIPosition();

	for (int i = 0 ; i <playerNumber; ++i)
	{
		m_pRenderTarget -> DrawBitmap(m_PlayerTurnTable[i]->GetPlayerFace(), m_ProfilePosition[i]);

		if (m_CurrentTurn == i)
			m_pRenderTarget -> DrawBitmap(m_PlayerTurnTable[i]->GetPlayerBox(), m_ProfileBoxPosition[i]);
		else
			m_pRenderTarget -> DrawBitmap(m_PlayerTurnTable[i]->GetPlayerWaitingBox(), m_ProfileBoxPosition[i]);
	}

	//m_Sprite->StartAnimation(m_ProfileBoxPosition[0]);

}


void CGameMap::Render()
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

	m_pRenderTarget->DrawBitmap(m_backImg,D2D1::RectF(0,0,WINDOW_WIDTH,WINDOW_HEIGHT));

	DrawPlayerUI(CGameData::GetInstance()->GetplayerNumber());

	/*	tile layer */
	for (int i = 0; i <= MAX_MAP_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_MAP_HEIGHT; ++j)
		{
			if (GetMapType(IndexedPosition(i, j) ) == MO_TILE)
			{
				m_pos.y = m_StartPosition.height + ( (m_LineWeight + m_TileSize) / 2 ) * (i -1) + m_LineWeight / 2;
				m_pos.x = m_StartPosition.width + ( (m_LineWeight + m_TileSize) / 2 ) * (j - 1) + m_LineWeight / 2;
				rectElement = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_TileSize / 2, m_pos.x + m_TileSize / 2, m_pos.y + m_TileSize / 2);

				switch (GetMapOwner(IndexedPosition(i, j) ) )
				{
				case MO_NOBODY:
					m_pRenderTarget->FillRectangle(rectElement, m_pVoidTileBrush);
					break;
				case MO_PLAYER1:
					m_pRenderTarget->FillRectangle(rectElement, m_PlayerTurnTable[0]->GetPlayerBrush() );
					break;
				case MO_PLAYER2:
					m_pRenderTarget->FillRectangle(rectElement, m_PlayerTurnTable[1]->GetPlayerBrush() );
					break;
				case MO_PLAYER3:
					m_pRenderTarget->FillRectangle(rectElement, m_PlayerTurnTable[2]->GetPlayerBrush() );
					break;
				case MO_PLAYER4:
					m_pRenderTarget->FillRectangle(rectElement, m_PlayerTurnTable[3]->GetPlayerBrush() );
					break;
				default:
					break;
				}
				
				//아직 애니메이션을 그릴 차례가 아니면 소유주가 없는 상태로 덮어 버린다.
				if ( (m_Map[i][j].m_AnimationTurn > m_TileAnimationTurn && m_TileAnimationTurn != 0)
					|| (m_Map[i][j].m_AnimationTurn != 0 && m_LineAnimationFlag) )
				{
					m_pRenderTarget->FillRectangle(rectElement, m_pVoidTileBrush);
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
						m_Map[i][j].m_StartTime = CGameTimer::GetInstance()->GetTime();
					}
					
					DWORD currentTime = CGameTimer::GetInstance()->GetTime();

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
						//애니메이션 재생이 완료되면 현재 타일 변수 초기화 및 다음 턴 그릴 준비
						m_TileAnimationTurn = m_Map[i][j].m_AnimationTurn + 1;
						m_Map[i][j].m_AnimationTurn = 0;
						m_Map[i][j].m_AnimationFlag = false;
						
						//혹시라도 애니메이션 재생 시간이 20초를 넘길 경우를 대비해서 주기적으로 초기화
						CGameTimer::GetInstance()->SetTimerStart();
						
						//타일 애니메이션 전체가 끝난거라면 관련 변수 초기화
						if (m_TileAnimationTurn > m_TileAnimationTurnNumber)
						{
							m_TileAnimationTurn = 0;
							m_TileAnimationTurnNumber = 0;
						}
					}
					else
					{
						//애니메이션이 재생 중이라면 
						D2D1_RECT_F animationRect;
						
						//애니메이션을 그린다
						switch (m_Map[i][j].m_Direction)
						{
						case DI_UP:
							animationRect = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_TileSize / 2, m_pos.x + m_TileSize / 2, m_pos.y + m_TileSize / 2 - tempLine);
							break;
						case DI_RIGHT:
							animationRect = D2D1::Rect( m_pos.x - m_TileSize / 2 + tempLine, m_pos.y - m_TileSize / 2, m_pos.x + m_TileSize / 2, m_pos.y + m_TileSize / 2);
							break;
						case DI_DOWN:
							animationRect = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_TileSize / 2 + tempLine, m_pos.x + m_TileSize / 2, m_pos.y + m_TileSize / 2);
							break;
						case DI_LEFT:
							animationRect = D2D1::Rect( m_pos.x - m_TileSize / 2, m_pos.y - m_TileSize / 2, m_pos.x + m_TileSize / 2 - tempLine, m_pos.y + m_TileSize / 2);
							break;
						default:
							break;
						}

						m_pRenderTarget->FillRectangle(animationRect, m_pVoidTileBrush);
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
							m_Map[i][j].m_StartTime = CGameTimer::GetInstance()->GetTime();
						}

						DWORD currentTime = CGameTimer::GetInstance()->GetTime();
						float timeWeight = currentTime - m_Map[i][j].m_StartTime;

						float tempLine = m_TileSize * ( timeWeight / SC_P_LINE_ANIMATION_TIME);

						if (tempLine > m_TileSize)
						{
							//line 애니메이션 종료되면 관련 변수 초기화
							m_Map[i][j].m_AnimationFlag = false;
							m_LineAnimationFlag = false;
							m_pRenderTarget->FillRectangle(rectElement, m_pConnectedLineBrush);

							//다음에 재생할 타일 애니메이션이 없으면 타이머 초기화해서 다음 턴 진행
							if (m_TileAnimationTurnNumber == 0)
							{
								CGameTimer::GetInstance()->SetTimerStart();
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
	rectElement = D2D1::Rect(m_pos.x - (m_TimerWidth / 2), m_pos.y, m_pos.x + (m_TimerWidth / 2), m_pos.y + m_TimerHeight);
	m_pRenderTarget->FillRectangle(rectElement, m_pUnconnectedLineBrush);

	float remainTimeRatio = (CGameTimer::GetInstance()->GetRemainTime() / static_cast<float>(TIME_LIMIT) );
	
	//animation 재생 중에는 타이머 안 줄어드는 것처럼 보이게 함
	if (m_LineAnimationFlag || m_TileAnimationTurn != 0)
	{
		remainTimeRatio = 1.0f;
	}

	float currentTimerLength = m_TimerWidth * remainTimeRatio;

	//남은 시간 표시 (브러시 새로 만들어 쓸 것)
	rectElement = D2D1::Rect(m_pos.x - (m_TimerWidth / 2), m_pos.y, m_pos.x - (m_TimerWidth / 2) + currentTimerLength, m_pos.y + m_TimerHeight);
	m_pRenderTarget->FillRectangle(rectElement, m_pTimer);
}

void CGameMap::SetMapSize(MapSize mapsize)
{
	m_MapSize.m_Width = mapsize.m_Width;
	m_MapSize.m_Height = mapsize.m_Height;
}

bool CGameMap::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(7.0f/255, 104.0f/255, 172.0f/255), &m_pDotBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(204.0f/255, 204.0f/255, 204.0f/255), &m_pUnconnectedLineBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(100.0f/255, 100.0f/255, 100.0f/255), &m_pPossibleLineBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(78.0f/255, 179.0f/255, 211.0f/255), &m_pConnectedLineBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &m_pVoidTileBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gold), &m_pGoldBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pTrashBrush);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
		
		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(212.0f/255, 72.0f/255, 101.0f/255), &m_pTimer);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();
	}

	m_backImg = CRenderer::GetInstance()->CreateImage(L"Resource/Image/background_game.png", m_backImg);

	m_gold = CRenderer::GetInstance()->CreateImage(L"Resource/Image/item1.png", m_gold);
	m_trash = CRenderer::GetInstance()->CreateImage(L"Resource/Image/item2.png", m_trash);

	return true;
}

void CGameMap::DrawLine(const IndexedPosition& indexedPosition)
{
	// 범위를 벗어난 경우 예외 처리
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_CONNECTED;
	
	//animaiton start
	m_LineAnimationFlag = true;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = true;
}

void CGameMap::DeleteLine( const IndexedPosition& indexedPosition )
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

void CGameMap::CalcStartPosition()
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

void CGameMap::ResizeClient()
{
	SetObjectSize();
	CalcStartPosition();
}

void CGameMap::SetObjectSize()
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
	m_TimerWidth = tempScale * SC_P_TIMER_WIDTH;
	m_TimerHeight = tempScale * SC_P_TIMER_HEIGHT;

	m_ProfileSize = tempScale * DEFAULT_CHARACTER_SIZE;
	m_ProfileBoxHeight = tempScale * DEFAULT_CHARACTER_BOX_HEIGHT;
	m_ProfileBoxWidth = tempScale * DEFAULT_CHARACTER_BOX_WIDTH;

	m_ProfileHorizontalMargin = tempScale * DEFAULT_CHARACTER_MARGIN_H;
	m_ProfileVerticalMargin = tempScale * DEFAULT_CHARACTER_MARGIN_V;
}

void CGameMap::GetPlayerUIPosition()
{
	m_ProfilePosition[0] = D2D1::RectF(m_CenterPosition.width - m_ProfileHorizontalMargin, m_CenterPosition.height-m_ProfileVerticalMargin,m_CenterPosition.width-m_ProfileHorizontalMargin+m_ProfileSize,m_CenterPosition.height-m_ProfileVerticalMargin+m_ProfileSize);
	m_ProfileBoxPosition[0] = D2D1::RectF(m_CenterPosition.width - m_ProfileHorizontalMargin, m_CenterPosition.height-m_ProfileVerticalMargin + m_ProfileSize ,m_CenterPosition.width-m_ProfileHorizontalMargin+m_ProfileSize,m_CenterPosition.height-m_ProfileVerticalMargin+m_ProfileSize + m_ProfileBoxHeight);

	m_ProfilePosition[1] = D2D1::RectF(m_CenterPosition.width + m_ProfileHorizontalMargin - m_ProfileSize, m_CenterPosition.height-m_ProfileVerticalMargin,m_CenterPosition.width + m_ProfileHorizontalMargin,m_CenterPosition.height-m_ProfileVerticalMargin+m_ProfileSize);
	m_ProfileBoxPosition[1] = D2D1::RectF(m_CenterPosition.width + m_ProfileHorizontalMargin - m_ProfileSize, m_CenterPosition.height-m_ProfileVerticalMargin + m_ProfileSize,m_CenterPosition.width + m_ProfileHorizontalMargin,m_CenterPosition.height-m_ProfileVerticalMargin+m_ProfileSize + m_ProfileBoxHeight);

	m_ProfilePosition[2] = D2D1::RectF(m_CenterPosition.width - m_ProfileHorizontalMargin, m_CenterPosition.height + m_ProfileVerticalMargin - m_ProfileSize, m_CenterPosition.width-m_ProfileHorizontalMargin+m_ProfileSize, m_CenterPosition.height + m_ProfileVerticalMargin);
	m_ProfileBoxPosition[2] = D2D1::RectF(m_CenterPosition.width - m_ProfileHorizontalMargin, m_CenterPosition.height + m_ProfileVerticalMargin, m_CenterPosition.width-m_ProfileHorizontalMargin+m_ProfileSize, m_CenterPosition.height + m_ProfileVerticalMargin + m_ProfileBoxHeight);

	m_ProfilePosition[3] = D2D1::RectF(m_CenterPosition.width + m_ProfileHorizontalMargin - m_ProfileSize, m_CenterPosition.height + m_ProfileVerticalMargin - m_ProfileSize, m_CenterPosition.width + m_ProfileHorizontalMargin, m_CenterPosition.height + m_ProfileVerticalMargin);
	m_ProfileBoxPosition[3] = D2D1::RectF(m_CenterPosition.width + m_ProfileHorizontalMargin - m_ProfileSize, m_CenterPosition.height + m_ProfileVerticalMargin, m_CenterPosition.width + m_ProfileHorizontalMargin, m_CenterPosition.height + m_ProfileVerticalMargin + m_ProfileBoxHeight);
}

MO_TYPE CGameMap::GetMapType(IndexedPosition indexedPosition)
{
	if (indexedPosition.m_PosI < MAX_MAP_HEIGHT && indexedPosition.m_PosJ < MAX_MAP_WIDTH)
	{
		return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type;
	}

	/*	게임이 진행되는 맵 영역 밖은 모두 MO_SENTINEL로 간주하므로 
		배열 범위 밖을 벗어나도 모두 MO_SENTINEL로 처리	*/
	return MO_SENTINEL;
}

MO_TYPE CGameMap::GetMapType(const int& i, const int& j)
{
	return GetMapType(IndexedPosition(i, j) );
}

MO_OWNER CGameMap::GetMapOwner( IndexedPosition indexedPosition )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner;
}

MO_OWNER CGameMap::GetMapOwner( const int& i , const int& j )
{
	return GetMapOwner( IndexedPosition(i, j) );
}

void CGameMap::SetMapOwner( IndexedPosition indexedPosition, MO_OWNER owner )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner = owner;
}

void CGameMap::SetItem( IndexedPosition indexedPosition, MO_ITEM item )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Item = item;
}

void CGameMap::SetMapFlag( IndexedPosition indexedPosition, bool flag )
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Flag = flag;
}

void CGameMap::WriteResult()
{
	for (int i = 2 ; i < MAX_MAP_HEIGHT; ++i)
	{
		for (int j = 2 ; j < MAX_MAP_WIDTH; ++j)
		{
			if ( GetMapType(i, j) == MO_TILE )
			{
				m_PlayerTurnTable[static_cast<int>(GetMapOwner(i, j) )]->UpdatePlayerResult( GetItem( IndexedPosition(i, j) ) );
			}
		}
	}
}

void CGameMap::InitVirtualLineState()
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

void CGameMap::ShowVirtualLine( const IndexedPosition& indexedPosition)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_MouseOverFlag = true;
}

void CGameMap::SetAnimationState(IndexedPosition indexedPosition, int turn, Direction direction)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = true;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn = turn;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Direction = direction;
}

void CGameMap::InitAnimationState(IndexedPosition indexedPosition)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn = 0;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = false;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Direction = DI_UP;
}

int	CGameMap::GetTileAnimationTurn(IndexedPosition indexedPosition)
{
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn;
}

void CGameMap::SetPlayerTurnTable(int idx, CPlayer* playerPtr)
{
	assert(idx >= 0 && idx < MAX_PLAYER_NUM);
	m_PlayerTurnTable[idx] = playerPtr;
}