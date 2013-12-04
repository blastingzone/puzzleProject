#include "stdafx.h"
#include "GameMap.h"
#include "GameTimer.h"

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

	m_pTileP1 = nullptr;
	m_pTileP2 = nullptr;
	m_pTileP3 = nullptr;
	m_pTileP4 = nullptr;

	memset(m_pPlayer,0,sizeof(m_pPlayer) );
	memset(m_pPlayerBox,0,sizeof(m_pPlayerBox) );

	m_LineAnimationFlag = false;
	m_TileAnimationTurnNumber = 0;
	m_TileAnimationTurn = 0;

	m_TileSize = 0;
	m_LineWeight = 0;
	m_DotRadius = 0;
	m_ItemRadius = 0;
	m_ProfileSize = 0;
	m_ProfileMargin = 0;
	m_ProfileBoxSize = 0;

	m_TimerPositionHeight = 0;
	m_TimerWidth = 0;
	m_TimerHeight = 0;

	//조심해!! GetMapSize를 아예 바꿔줄거야.
	m_MapSize.m_Width = mapSize.m_Width;
	m_MapSize.m_Height = mapSize.m_Height;

	m_VoidTileCount = m_MapSize.m_Width * m_MapSize.m_Height;

	//조심해!!
	//초기화해서 화면 스케일에 맞춰서 적용할 것
	m_ProfileSize = 150.0f;
	m_ProfileMargin = 50.0f;
	m_ProfileBoxSize = 30.0f;

	m_isMouseOn = false;
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

	SafeRelease(m_pTileP1);
	SafeRelease(m_pTileP2);
	SafeRelease(m_pTileP3);
	SafeRelease(m_pTileP4);
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
					//조심해 !!!x10000
					//이거 무서운 코드다 m_MapSize 크기가 어떻게 되든 m_Map은 21x21로 고정 되어 있다.
					//아무리 하드 코딩하더라도 이렇게 놔두지 않는 습관을 들여야 한다. 
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

	return true;
}

void CGameMap::DrawPlayerUI( int playerNumber )
{
	//조심해!!
	//일반화해서 포문으로 돌릴 수 있을 것 같아!!
	if (playerNumber >= 2)
	{
		m_pRenderTarget->DrawBitmap(
			m_pPlayer[0], 
			D2D1::RectF(
				m_ProfileMargin, 
				m_ProfileMargin, 
				m_ProfileMargin + m_ProfileSize,
				m_ProfileMargin + m_ProfileSize
				) 
			);
		
		m_pRenderTarget->DrawBitmap(
			m_pPlayerBox[0],
			D2D1::RectF(
				m_ProfileMargin, 
				m_ProfileMargin + m_ProfileSize, 
				m_ProfileMargin + m_ProfileSize, 
				m_ProfileMargin + m_ProfileSize + m_ProfileBoxSize
				)
			);

		m_pRenderTarget->DrawBitmap(
			m_pPlayer[1],
			D2D1::RectF(
				WINDOW_WIDTH - m_ProfileSize - m_ProfileMargin, 
				m_ProfileMargin, 
				WINDOW_WIDTH - m_ProfileMargin,
				m_ProfileMargin + m_ProfileSize
				)
			);

		m_pRenderTarget->DrawBitmap(
			m_pPlayerBox[1],
			D2D1::RectF(
				WINDOW_WIDTH - m_ProfileSize - m_ProfileMargin, 
				m_ProfileMargin + m_ProfileSize, 
				WINDOW_WIDTH - m_ProfileMargin, 
				m_ProfileMargin + m_ProfileSize + m_ProfileBoxSize
				)
			);
	}

	if (playerNumber >= 3)
	{
		m_pRenderTarget->DrawBitmap(
			m_pPlayer[2],
			D2D1::RectF(
				m_ProfileMargin,
				WINDOW_HEIGHT - m_ProfileSize - m_ProfileMargin,
				m_ProfileMargin + m_ProfileSize,
				WINDOW_HEIGHT - m_ProfileMargin
				)
			);

		m_pRenderTarget->DrawBitmap(
			m_pPlayerBox[2],
			D2D1::RectF(
				m_ProfileMargin,
				WINDOW_HEIGHT - m_ProfileMargin,
				m_ProfileMargin + m_ProfileSize,
				WINDOW_HEIGHT - m_ProfileMargin + m_ProfileBoxSize
				)
			);
	}

	if (playerNumber == 4)
	{
		m_pRenderTarget->DrawBitmap(
			m_pPlayer[3],
			D2D1::RectF(
				WINDOW_WIDTH - m_ProfileSize - m_ProfileMargin,
				WINDOW_HEIGHT - m_ProfileSize - m_ProfileMargin,
				WINDOW_WIDTH - m_ProfileMargin,
				WINDOW_HEIGHT - m_ProfileMargin
				)
			);

		m_pRenderTarget->DrawBitmap(
			m_pPlayerBox[3],
			D2D1::RectF(
				WINDOW_WIDTH - m_ProfileSize - m_ProfileMargin,
				WINDOW_HEIGHT - m_ProfileMargin,
				WINDOW_WIDTH - m_ProfileMargin,
				WINDOW_HEIGHT - m_ProfileMargin + m_ProfileBoxSize
				)
			);
	}
	//m_pRenderTarget->FillRectangle(D2D1::RectF(50.0f,50.0f+m_ProfileSize,50.0f+m_ProfileSize,80.0f+m_ProfileSize), m_pPlayerBox);
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
		(순서대로 애니메이션이 지정된 타일들의 재생이 완료되어야 전체 타일 애니메이션 종료) */

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
					m_pRenderTarget->FillRectangle(rectElement, m_pTileP1);
					break;
				case MO_PLAYER2:
					m_pRenderTarget->FillRectangle(rectElement, m_pTileP2);
					break;
				case MO_PLAYER3:
					m_pRenderTarget->FillRectangle(rectElement, m_pTileP3);
					break;
				case MO_PLAYER4:
					m_pRenderTarget->FillRectangle(rectElement, m_pTileP4);
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
						
						//조심해!
						//애니메이션 재생 중에는 타이머 일시 정지 기능 추가 필요할지도?
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

	//타이머 작업 중
	D2D1_SIZE_F centerPosition;
	centerPosition = m_pRenderTarget->GetSize();

	centerPosition.width /= 2;

	m_pos.x = centerPosition.width;
	m_pos.y = m_TimerPositionHeight;

	//배경
	rectElement = D2D1::Rect(m_pos.x - (m_TimerWidth / 2), m_pos.y, m_pos.x + (m_TimerWidth / 2), m_pos.y + m_TimerHeight);
	m_pRenderTarget->FillRectangle(rectElement, m_pUnconnectedLineBrush);

	float remainTimeRatio = (CGameTimer::GetInstance()->GetRemainTime() / static_cast<float>(TIME_LIMIT) );
	float currentTimerLength = m_TimerWidth * remainTimeRatio;

	//남은 시간 표시
	rectElement = D2D1::Rect(m_pos.x - (m_TimerWidth / 2), m_pos.y, m_pos.x - (m_TimerWidth / 2) + currentTimerLength, m_pos.y + m_TimerHeight);
	m_pRenderTarget->FillRectangle(rectElement, m_pTileP3);
}

void CGameMap::SetMapSize(MapSize mapsize)
{
	m_MapSize.m_Width = mapsize.m_Width;
	m_MapSize.m_Height = mapsize.m_Height;
}

bool CGameMap::CreateResource()
{
	// 조심해!
	// 현재 주석 처리된 색들 나중에 정리해서 define으로 따로 저장하고 쓸 것
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(7.0f/255, 104.0f/255, 172.0f/255), &m_pDotBrush);
		//hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkMagenta), &m_pDotBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(204.0f/255, 204.0f/255, 204.0f/255), &m_pUnconnectedLineBrush);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(115.0f/255, 191.0f/255, 31.0f/255), &m_pUnconnectedLineBrush); //ver 1106
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gainsboro), &m_pUnconnectedLineBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(100.0f/255, 100.0f/255, 100.0f/255), &m_pPossibleLineBrush);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(115.0f/255, 191.0f/255, 31.0f/255), &m_pUnconnectedLineBrush); //ver 1106
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gainsboro), &m_pUnconnectedLineBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(78.0f/255, 179.0f/255, 211.0f/255), &m_pConnectedLineBrush);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(30.0f/255, 153.0f/255, 197.0f/255), &m_pConnectedLineBrush); //ver 1106
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Tomato), &m_pConnectedLineBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &m_pVoidTileBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gold), &m_pGoldBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pTrashBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_1_), &m_pTileP1);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(204.0f/255, 232.0f/255, 36.0f/255), &m_pTileP1); //ver 1106
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(78.0f/255, 183.0f/255, 153.0f/255),&m_pTileP1);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pTileP1);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_2_), &m_pTileP2);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &m_pTileP2);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_3_), &m_pTileP3);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CadetBlue), &m_pTileP3);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_4_), &m_pTileP4);
		//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DeepPink), &m_pTileP4);

		if (!SUCCEEDED(hr) )
			return false;
	}

	for (int turn = 0; turn < CGameData::GetInstance()->GetplayerNumber(); ++turn)
	{
		m_pPlayer[turn] = CRenderer::GetInstance()->CreateImage(
			CGameData::GetInstance()->GetPlayerImage(turn), 
			m_pPlayer[turn]
		);

		m_pPlayerBox[turn] = CRenderer::GetInstance()->CreateImage(
			CGameData::GetInstance()->GetPlayerBox(turn), 
			m_pPlayerBox[turn]
		);

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
	D2D1_SIZE_F centerPosition;
	centerPosition = m_pRenderTarget->GetSize();

	centerPosition.height /= 2;
	centerPosition.width /= 2;

	m_StartPosition.height = 
		centerPosition.height 
		- (m_MapSize.m_Height * (m_LineWeight + m_TileSize) + m_LineWeight) / 2;

	m_StartPosition.width = 
		centerPosition.width 
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

	m_TileSize = tempScale * DEFAULT_TILE_SIZE;
	m_LineWeight = tempScale * DEFAULT_LINE_WEIGHT;
	m_DotRadius = tempScale * DEFAULT_DOT_RADIUS;
	m_ItemRadius = tempScale * DEFAULT_ITEM_RADIUS;

	m_TimerPositionHeight = tempScale * SC_P_TIMER_POSITION_HEIGHT;
	m_TimerWidth = tempScale * SC_P_TIMER_WIDTH;
	m_TimerHeight = tempScale * SC_P_TIMER_HEIGHT;
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
	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner;
}

MO_OWNER CGameMap::GetMapOwner( const int& i , const int& j )
{
	return GetMapOwner( IndexedPosition(i, j) );
}

void CGameMap::SetMapOwner( IndexedPosition indexedPosition, MO_OWNER owner )
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner = owner;
}

void CGameMap::SetItem( IndexedPosition indexedPosition, MO_ITEM item )
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Item = item;
}

void CGameMap::SetMapFlag( IndexedPosition indexedPosition, bool flag )
{
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
				CGameData::GetInstance()->UpdatePlayerResult( GetMapOwner(i, j), GetItem(IndexedPosition(i, j)) );
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
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_MouseOverFlag = true;
}

void CGameMap::SetAnimationState(IndexedPosition indexedPosition, int turn, Direction direction)
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = true;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn = turn;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Direction = direction;
}

void CGameMap::InitAnimationState(IndexedPosition indexedPosition)
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn = 0;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = false;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Direction = DI_UP;
}

int	CGameMap::GetTileAnimationTurn(IndexedPosition indexedPosition)
{
	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationTurn;
}