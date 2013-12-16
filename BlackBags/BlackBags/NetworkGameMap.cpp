#include "stdafx.h"
#include "NetworkGameMap.h"
#include "GameTimer.h"
#include "Player.h"

#define _USE_MATH_DEFINES
#include <cmath>

CNetworkGameMap::CNetworkGameMap(MapSize mapSize)
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

CNetworkGameMap::~CNetworkGameMap(void)
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
}


void CNetworkGameMap::CreateMap()
{
	/*	������ ���ӿ� ���Ǵ� Ÿ�� �ܿ��� ��Ÿ���� ���� ǥ���ϱ� ���� ĭ�� �ʿ�
		������ ���� �ֺ��� �⺻���� MO_SENTINEL�� �ξ IsClosed()�� ���� �۾��� Ȱ���� �� �ֵ��� �� */
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

void CNetworkGameMap::DrawPlayerUI( int playerNumber )
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
}


void CNetworkGameMap::Render()
{
	D2D1_ELLIPSE	m_DotEllipse;
	D2D1_RECT_F		rectElement;
	D2D1_POINT_2F	m_pos;

	/*	�ִϸ��̼� �߰�
		�⺻������ �ִϸ��̼� flag�� line�� tile �� ������ ����
		�� �߿��� �ϳ��� ���� ��(flag�� true)�̸� ���콺 �Է��� ���� �ʰ� �ִϸ��̼��� �Ϸ� �Ǿ�� ���� ����
		�ִϸ��̼� ������ line >> tile
		Ÿ�� �ִϸ��̼ǿ��� ����Ǵ� ������ �����Ǿ� ����
		(�ִϸ��̼��� ������ Ÿ�ϵ��� ����� ��� �Ϸ�Ǿ�� ��ü Ÿ�� �ִϸ��̼� ����) */

	/*	layer : background - tile - line - dot ������� ������ 
		����� �������� ǥ�õǴ� ���� �����Ƿ� �ʿ����
		���� �����۰� UI �߰� �� ��Ȳ�� ���缭 ���̾� ���� ���� �� �� */

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
				
				//���� �ִϸ��̼��� �׸� ���ʰ� �ƴϸ� �����ְ� ���� ���·� ���� ������.
				if ( (m_Map[i][j].m_AnimationTurn > m_TileAnimationTurn && m_TileAnimationTurn != 0)
					|| (m_Map[i][j].m_AnimationTurn != 0 && m_LineAnimationFlag) )
				{
					m_pRenderTarget->FillRectangle(rectElement, m_pVoidTileBrush);
				}
				//�������� ���� �ִϸ��̼��� ���� 
				//�ִϸ��̼� �׸� ���� �Ǿ��ų� ���� ���̶��
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
					
					//���� �ִϸ��̼� ��� �ð��� �ʰ��ϸ� �ִ� ũ��� �Ҵ�
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
						//�ִϸ��̼� ����� �Ϸ�Ǹ� ���� Ÿ�� ���� �ʱ�ȭ �� ���� �� �׸� �غ�
						m_TileAnimationTurn = m_Map[i][j].m_AnimationTurn + 1;
						m_Map[i][j].m_AnimationTurn = 0;
						m_Map[i][j].m_AnimationFlag = false;
						
						//Ȥ�ö� �ִϸ��̼� ��� �ð��� 20�ʸ� �ѱ� ��츦 ����ؼ� �ֱ������� �ʱ�ȭ
						CGameTimer::GetInstance()->SetTimerStart();
						
						//Ÿ�� �ִϸ��̼� ��ü�� �����Ŷ�� ���� ���� �ʱ�ȭ
						if (m_TileAnimationTurn > m_TileAnimationTurnNumber)
						{
							m_TileAnimationTurn = 0;
							m_TileAnimationTurnNumber = 0;
						}
					}
					else
					{
						//�ִϸ��̼��� ��� ���̶�� 
						D2D1_RECT_F animationRect;
						
						//�ִϸ��̼��� �׸���
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
					//GET - SET function ���� ��
					if (m_Map[i][j].m_AnimationFlag)
					{
						//UnconnectedLine�� ��濡 �ΰ� ���� �׷��� �ϹǷ� ����� ���� �׸�
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
							//line �ִϸ��̼� ����Ǹ� ���� ���� �ʱ�ȭ
							m_Map[i][j].m_AnimationFlag = false;
							m_LineAnimationFlag = false;
							m_pRenderTarget->FillRectangle(rectElement, m_pConnectedLineBrush);

							//������ ����� Ÿ�� �ִϸ��̼��� ������ Ÿ�̸� �ʱ�ȭ�ؼ� ���� �� ����
							if (m_TileAnimationTurnNumber == 0)
							{
								CGameTimer::GetInstance()->SetTimerStart();
							}
						}
						else
						{
							//�ִϸ��̼� ���
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

	//���
	rectElement = D2D1::Rect(m_pos.x - (m_TimerWidth / 2), m_pos.y, m_pos.x + (m_TimerWidth / 2), m_pos.y + m_TimerHeight);
	m_pRenderTarget->FillRectangle(rectElement, m_pUnconnectedLineBrush);

	float remainTimeRatio = (CGameTimer::GetInstance()->GetRemainTime() / static_cast<float>(TIME_LIMIT) );
	
	//animation ��� �߿��� Ÿ�̸� �� �پ��� ��ó�� ���̰� ��
	if (m_LineAnimationFlag || m_TileAnimationTurn != 0)
	{
		remainTimeRatio = 1.0f;
	}

	float currentTimerLength = m_TimerWidth * remainTimeRatio;

	//���� �ð� ǥ�� (�귯�� ���� ����� �� ��)
	rectElement = D2D1::Rect(m_pos.x - (m_TimerWidth / 2), m_pos.y, m_pos.x - (m_TimerWidth / 2) + currentTimerLength, m_pos.y + m_TimerHeight);
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

void CNetworkGameMap::DrawLine(const IndexedPosition& indexedPosition)
{
	// ������ ��� ��� ���� ó��
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_CONNECTED;
	
	//animaiton start
	m_LineAnimationFlag = true;
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_AnimationFlag = true;
}

void CNetworkGameMap::DeleteLine( const IndexedPosition& indexedPosition )
{
	// ������ ��� ��� ���� ó��
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	//���� ���� �ߴ� ���������� �ִϸ��̼� ���� ��ȭ �ǵ�����
	for (int tempI = 0 ; tempI < MAX_MAP_WIDTH; ++tempI)
	{
		for (int tempJ = 0 ; tempJ < MAX_MAP_HEIGHT; ++tempJ)
		{
			SetMapFlag(IndexedPosition(tempI, tempJ), false);

			if (GetMapType(IndexedPosition(tempI, tempJ) ) == MO_TILE)
			{
				//�ִϸ��̼� ����� ���� �����͵� �ʱ�ȭ
				InitAnimationState(IndexedPosition(tempI, tempJ) );
			}
		}
	}

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_UNCONNECTED;
}

void CNetworkGameMap::CalcStartPosition()
{
	/*	���� ȭ���� �߽����� �������� ������ ���� ũ���� �ݸ�ŭ
		���ʰ� �������� �̵��� ������ m_StartPosition���� ���� */
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
	SetObjectSize();
	CalcStartPosition();
}

void CNetworkGameMap::SetObjectSize()
{
	/*	���� �������� ����� ȭ�� �����Ͽ� ���缭 
		������ �� �� ���� ������Ʈ�� ũ�� ���� */
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_TileSize = tempScale * DEFAULT_TILE_SIZE;
	m_LineWeight = tempScale * DEFAULT_LINE_WEIGHT;
	m_DotRadius = tempScale * DEFAULT_DOT_RADIUS;
	m_ItemRadius = tempScale * DEFAULT_ITEM_RADIUS;

	m_TimerPositionHeight = tempScale * SC_P_TIMER_POSITION_HEIGHT;
	m_TimerWidth = tempScale * SC_P_TIMER_WIDTH;
	m_TimerHeight = tempScale * SC_P_TIMER_HEIGHT;

	m_ProfileSize = tempScale * DEFAULT_CHARACTER_SIZE;
	m_ProfileBoxHeight = tempScale * DEFAULT_CHARACTER_BOX_HEIGHT;
	m_ProfileBoxWidth = tempScale * DEFAULT_CHARACTER_BOX_WIDTH;

	m_ProfileHorizontalMargin = tempScale * DEFAULT_CHARACTER_MARGIN_H;
	m_ProfileVerticalMargin = tempScale * DEFAULT_CHARACTER_MARGIN_V;
}

void CNetworkGameMap::GetPlayerUIPosition()
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

MO_TYPE CNetworkGameMap::GetMapType(IndexedPosition indexedPosition)
{
	if (indexedPosition.m_PosI < MAX_MAP_HEIGHT && indexedPosition.m_PosJ < MAX_MAP_WIDTH)
	{
		return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type;
	}

	/*	������ ����Ǵ� �� ���� ���� ��� MO_SENTINEL�� �����ϹǷ� 
		�迭 ���� ���� ����� ��� MO_SENTINEL�� ó��	*/
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
				m_PlayerTurnTable[static_cast<int>(GetMapOwner(i, j) )]->UpdatePlayerResult( GetItem( IndexedPosition(i, j) ) );
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
	m_PlayerTurnTable[idx] = playerPtr;
}