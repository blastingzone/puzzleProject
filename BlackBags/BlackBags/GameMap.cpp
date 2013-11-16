#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"

CGameMap::CGameMap(void)
{
	m_pRenderTarget = nullptr;

	//조심해!! GetMapSize를 아예 바꿔줄거야.
	m_MapSize.m_Width = 9;
	m_MapSize.m_Height = 8;

	m_VoidTileCount = m_MapSize.m_Width * m_MapSize.m_Height;
}

CGameMap::~CGameMap(void)
{
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

void CGameMap::Init()
{
	CreateResource();
	SetObjectSize();
	ResizeClient();
	SetMapSize(m_MapSize);
	CreateMap();

	return;
}

void CGameMap::Render()
{
	D2D1_ELLIPSE	m_DotEllipse;
	D2D1_RECT_F		rectElement;
	D2D1_POINT_2F	m_pos;

	/*	layer : background - tile - line - dot 순서대로 렌더링 
		현재는 겹쳐져서 표시되는 것이 없으므로 필요없음
		추후 아이템과 UI 추가 시 상황에 맞춰서 레이어 구분 새로 할 것 */

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
			}
		}
	}
	
	/*	line layer */
	for (int i = 0; i <= MAX_MAP_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_MAP_HEIGHT; ++j)
		{
			if ( GetMapType(IndexedPosition(i, j) ) == MO_LINE_UNCONNECTED || GetMapType(IndexedPosition(i, j) ) == MO_LINE_CONNECTED )
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
					m_pRenderTarget->FillRectangle(rectElement, m_pConnectedLineBrush);
					break;
				default:
					break;
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
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(78.0f/255, 179.0f/255, 211.0f/255), &m_pConnectedLineBrush);
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(30.0f/255, 153.0f/255, 197.0f/255), &m_pConnectedLineBrush); //ver 1106
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Tomato), &m_pConnectedLineBrush);
		
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue), &m_pVoidTileBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(78.0f/255, 213.0f/255, 199.0f/255), &m_pTileP1);
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(204.0f/255, 232.0f/255, 36.0f/255), &m_pTileP1); //ver 1106
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(78.0f/255, 183.0f/255, 153.0f/255),&m_pTileP1);
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &m_pTileP1);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(247.0f/255, 166.0f/255, 123.0f/255), &m_pTileP2);
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &m_pTileP2);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(212.0f/255, 72.0f/255, 101.0f/255), &m_pTileP3);
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CadetBlue), &m_pTileP3);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(184.0f/255, 218.0f/255, 141.0f/255), &m_pTileP4);
			//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DeepPink), &m_pTileP4);

		if (SUCCEEDED(hr) )
			return true;
	}

	return false;
}

void CGameMap::DrawLine(const IndexedPosition& indexedPosition)
{
	// 범위를 벗어난 경우 예외 처리
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_CONNECTED;
}

void CGameMap::DeleteLine( const IndexedPosition& indexedPosition )
{
	// 범위를 벗어난 경우 예외 처리
	assert(indexedPosition.m_PosI < MAX_MAP_WIDTH && indexedPosition.m_PosJ<MAX_MAP_HEIGHT) ;

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
	CalcStartPosition();
	SetObjectSize();
}

void CGameMap::SetObjectSize()
{
	/*	현재 렌더러에 저장된 화면 스케일에 맞춰서 
		렌더링 할 때 사용된 오브젝트들 크기 조정 */
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_TileSize = tempScale * DEFAULT_TILE_SIZE;
	m_LineWeight = tempScale * DEFAULT_LINE_WEIGHT;
	m_DotRadius = tempScale * DEFAULT_DOT_RADIUS;
}

MO_TYPE CGameMap::GetMapType(IndexedPosition indexedPosition)
{
	if (indexedPosition.m_PosI < MAX_MAP_HEIGHT && indexedPosition.m_PosJ < MAX_MAP_WIDTH)
	{
		return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type;
	}

	/* 게임이 진행되는 맵 영역 밖은 모두 MO_SENTINEL로 간주하므로 
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

