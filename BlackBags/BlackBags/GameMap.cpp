#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"
#include "MacroSet.h"

//CGameMap* CGameMap::m_pInstance = nullptr;

CGameMap::CGameMap(void)
{
	m_pRenderTarget = nullptr;

	m_MapSize.m_Width = 5;
	m_MapSize.m_Height = 5;

	m_VoidTileCount = m_MapSize.m_Width * m_MapSize.m_Height;
}

CGameMap::~CGameMap(void)
{
	//SafeArrayDelete(m_Map);
}
/*
CGameMap* CGameMap::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameMap();
	}

	return m_pInstance;
}

bool CGameMap::ReleaseInstance()
{
	m_pInstance->Release();
	delete m_pInstance;

	return true;
}
*/
bool CGameMap::Release()
{
	SafeRelease(m_pDotBrush);
	SafeRelease(m_pUnconnectedLineBrush);
	SafeRelease(m_pConnectedLineBrush);
	SafeRelease(m_pPossibleLineBrush);
	SafeRelease(m_pTileBrush);
	SafeRelease(m_pVoidTileBrush);

	SafeRelease(m_pTileP1);
	SafeRelease(m_pTileP2);
	SafeRelease(m_pTileP3);
	SafeRelease(m_pTileP4);

	return true;
}

void CGameMap::CreateMap()
{
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
	CalcStartPosition();
	SetMapSize(m_MapSize);
	CreateMap();

	return;
}

void CGameMap::Render()
{
	D2D1_ELLIPSE	m_DotEllipse;
	D2D1_RECT_F		rectElement;
	D2D1_POINT_2F	m_pos;

	//layer : background - tile - line - dot

	//tile
	for (int i = 0; i <= MAX_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_HEIGHT; ++j)
		{
			if (GetMapType(i, j) == MO_TILE)
			{
				m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i -1) + LINE_WEIGHT / 2;
				m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j - 1) + LINE_WEIGHT / 2;
				rectElement = D2D1::Rect( m_pos.x - TILE_SIZE / 2, m_pos.y - TILE_SIZE / 2, m_pos.x + TILE_SIZE / 2, m_pos.y + TILE_SIZE / 2);
			
				switch (GetMapOwner(i, j) )
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
	
	//line
	for (int i = 0; i <= MAX_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_HEIGHT; ++j)
		{
			if ( GetMapType(i, j) == MO_LINE_UNCONNECTED || GetMapType(i, j) == MO_LINE_CONNECTED )
			{
				if (i%2==0)
				{
					m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i - 1) + LINE_WEIGHT / 2;
					m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j - 1) + LINE_WEIGHT / 2;
					rectElement = D2D1::Rect( m_pos.x - LINE_WEIGHT / 2, m_pos.y - TILE_SIZE / 2, m_pos.x + LINE_WEIGHT / 2, m_pos.y + TILE_SIZE / 2);
				}
				else
				{
					m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i - 1) + LINE_WEIGHT / 2;
					m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j - 1) + LINE_WEIGHT / 2;
					rectElement = D2D1::Rect( m_pos.x - TILE_SIZE / 2, m_pos.y - LINE_WEIGHT / 2, m_pos.x + TILE_SIZE / 2, m_pos.y + LINE_WEIGHT / 2);
				}
				switch (GetMapType(i, j) )
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

	//dot
	for (int i = 0; i <= MAX_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_HEIGHT; ++j)
		{
			if (GetMapType(i,j) == MO_DOT)
			{
				m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j - 1) + LINE_WEIGHT / 2;
				m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i - 1) + LINE_WEIGHT / 2;
				
				m_DotEllipse = D2D1::Ellipse( m_pos, DOT_RADIUS, DOT_RADIUS );
				m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pDotBrush);
			}
		}
	}

	//dev tool : check the start / end Points
	rectElement = D2D1::Rect( 
		m_StartPosition.width, 
		m_StartPosition.height, 
		m_StartPosition.width + m_MapSize.m_Width * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT, 
		m_StartPosition.height + m_MapSize.m_Height * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT);

	m_pRenderTarget->DrawRectangle(rectElement,m_pConnectedLineBrush);
}

bool CGameMap::SetMapSize(MapSize mapsize)
{
	m_MapSize.m_Width = mapsize.m_Width;
	m_MapSize.m_Height = mapsize.m_Height;

	return true;
}

MO_TYPE CGameMap::GetMapType(IndexedPosition indexedPosition)
{
	if (indexedPosition.m_PosI <= MAX_HEIGHT && indexedPosition.m_PosJ <= MAX_WIDTH)
	{
		return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type;
	}

	return MO_SENTINEL;
}

MO_TYPE CGameMap::GetMapType(const int& iPos, const int& jPos)
{
	return GetMapType(IndexedPosition(iPos, jPos) );
}

D2D1_SIZE_F CGameMap::GetStartPosition(){
	return m_StartPosition;
}

bool CGameMap::CreateResource()
{
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

	//
	return false;
}

void CGameMap::DrawLine(const IndexedPosition& indexedPosition)
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_CONNECTED;
}
void CGameMap::DrawLine(const int& iPos, const int& jPos)
{
	DrawLine(IndexedPosition(iPos, jPos) );
}

void CGameMap::CalcStartPosition()
{
	D2D1_SIZE_F centerPosition;
	centerPosition = m_pRenderTarget->GetSize();

	centerPosition.height /= 2;
	centerPosition.width /= 2;

	m_StartPosition.height = 
		centerPosition.height 
		- (m_MapSize.m_Height * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT) / 2;

	m_StartPosition.width = 
		centerPosition.width 
		- (m_MapSize.m_Width * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT) / 2;
}

void CGameMap::ResizeClient()
{
		CalcStartPosition();
}

MO_OWNER CGameMap::GetMapOwner( IndexedPosition indexedPosition )
{
	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner;
}

MO_OWNER CGameMap::GetMapOwner( const int& iPos, const int& jPos )
{
	return GetMapOwner(IndexedPosition(iPos,jPos));
}

void CGameMap::SetMapOwner( IndexedPosition indexedPosition, MO_OWNER owner )
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Owner = owner;
}

void CGameMap::SetMapOwner( const int& iPos, const int& jPos, MO_OWNER owner )
{
	SetMapOwner(IndexedPosition(iPos,jPos), owner);
}

void CGameMap::SetItem( IndexedPosition indexedPosition, MO_ITEM item )
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Item = item;
}

void CGameMap::SetItem(const int& iPos, const int& jPos, MO_ITEM item )
{
	SetItem(IndexedPosition(iPos,jPos),item);
}

void CGameMap::SetMapFlag( IndexedPosition indexedPosition, bool flag )
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Flag = flag;
}

void CGameMap::DeleteLine( const IndexedPosition& indexedPosition )
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Type = MO_LINE_UNCONNECTED;
}


void CGameMap::DeleteLine( const int& iPos, const int& jPos )
{
	DeleteLine( IndexedPosition(iPos, jPos) );
}