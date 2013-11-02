#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"
#include "MacroSet.h"

CGameMap* CGameMap::m_pInstance = nullptr;

CGameMap::CGameMap(void)
{
	memset(m_Map, 0, sizeof(m_Map)); 
	m_pRenderTarget = nullptr;

	m_MapSize.m_Width = 5;
	m_MapSize.m_Height = 5;
}

CGameMap::~CGameMap(void)
{
	//SafeArrayDelete(m_Map);
}

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

bool CGameMap::Release()
{
	SafeRelease(m_pDotBrush);
	SafeRelease(m_pUnconnectedLineBrush);
	SafeRelease(m_pConnectedLineBrush);
	SafeRelease(m_pPossibleLineBrush);
	SafeRelease(m_pTileBrush);
	SafeRelease(m_pVoidTileBrush);

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
					m_Map[targetRow][targetColumn] = MO_DOT;
				} 
				else 
				{
					m_Map[targetRow][targetColumn] = MO_LINE_UNCONNECTED;
				}
			} 
			else 
			{
				// line - tile - line - tile
				if (targetColumn % 2 == 1)
				{
					m_Map[targetRow][targetColumn] = MO_LINE_UNCONNECTED;
				} 
				else 
				{
					m_Map[targetRow][targetColumn] = MO_TILE_VOID;
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

bool CGameMap::IsPossible(IndexedPosition indexedPosition)
{
	if (m_pInstance->GetMapType(indexedPosition.m_PosI, indexedPosition.m_PosJ) == MO_LINE_UNCONNECTED)
	{
		//나중에 양 옆 타일의 소유주가 있는지 확인하는 조건도 추가할 것
		return true;
	}

	return false;
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
			m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i -1) + LINE_WEIGHT / 2;
			m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j - 1) + LINE_WEIGHT / 2;
			rectElement = D2D1::Rect( m_pos.x - TILE_SIZE / 2, m_pos.y - TILE_SIZE / 2, m_pos.x + TILE_SIZE / 2, m_pos.y + TILE_SIZE / 2);
			
			if (m_pInstance->GetMapType(i,j) == MO_TILE_VOID)
			{
				m_pRenderTarget->FillRectangle(rectElement, m_pVoidTileBrush);
			}
			if (m_pInstance->GetMapType(i,j) == MO_TILE_VOID_P1)
			{
				m_pRenderTarget->FillRectangle(rectElement, m_pTileP1);
			}
		}
	}
	
	//line
	for (int i = 0; i <= MAX_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_HEIGHT; ++j)
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
			switch (m_pInstance->GetMapType(i,j))
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

	//dot
	for (int i = 0; i <= MAX_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_HEIGHT; ++j)
		{
			if (m_pInstance->GetMapType(i,j) == MO_DOT)
			{
				m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i - 1) + LINE_WEIGHT / 2;
				m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j - 1) + LINE_WEIGHT / 2;
				m_DotEllipse = D2D1::Ellipse( m_pos, DOT_RADIUS, DOT_RADIUS );
				m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pDotBrush);
			}
		}
	}

	//dev tool : check the startPoint
	rectElement = D2D1::Rect( 
		m_StartPosition.width, 
		m_StartPosition.height, 
		m_StartPosition.width + m_MapSize.m_Height * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT, 
		m_StartPosition.height + m_MapSize.m_Width * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT);

	m_pRenderTarget->DrawRectangle(rectElement,m_pConnectedLineBrush);
}

bool CGameMap::SetMapSize(MapSize mapsize)
{
	m_MapSize.m_Width = mapsize.m_Width;
	m_MapSize.m_Height = mapsize.m_Height;

	return true;
}

MapObject CGameMap::GetMapType(IndexedPosition indexedPosition)
{
	if (indexedPosition.m_PosI <= MAX_HEIGHT && indexedPosition.m_PosJ <= MAX_WIDTH)
	{
		return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ];
	}
	//////////////////////////////////////////////////////////////////////////
	//조심해 oo 해결
	return MO_SENTINEL;
}

MapObject CGameMap::GetMapType(int iPos, int jPos)
{
	IndexedPosition tempPos;
	tempPos.m_PosI = iPos;
	tempPos.m_PosJ = jPos;

	return GetMapType(tempPos);
}

void CGameMap::SetMapType(IndexedPosition indexedPosition, MapObject newMapObject)
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ] = newMapObject;

	return;
}

void CGameMap::SetMapType(int iPos, int jPos, MapObject newMapObject)
{
	IndexedPosition tempPos;
	tempPos.m_PosI = iPos;
	tempPos.m_PosJ = jPos;

	SetMapType(tempPos, newMapObject);

	return;
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

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkMagenta),&m_pDotBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gainsboro),&m_pUnconnectedLineBrush);

		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Tomato),&m_pConnectedLineBrush);
		
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue),&m_pVoidTileBrush);
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow),&m_pTileP1);
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green),&m_pTileP2);
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CadetBlue),&m_pTileP3);
		if (SUCCEEDED(hr) )
			m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DeepPink),&m_pTileP4);

		if (SUCCEEDED(hr) )
			return true;
	}

	//
	return false;
}

void CGameMap::DrawLine(IndexedPosition indexedPosition)
{
	m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ] = MO_LINE_CONNECTED;
}
void CGameMap::DrawLine(int iPos, int jPos)
{
	IndexedPosition indexedPosition;
	indexedPosition.m_PosI=iPos;
	indexedPosition.m_PosJ =jPos;

	DrawLine(indexedPosition);
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

