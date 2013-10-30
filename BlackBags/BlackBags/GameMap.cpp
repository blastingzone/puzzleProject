#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"
#include "MacroSet.h"

CGameMap* CGameMap::m_pInstance = nullptr;

CGameMap::CGameMap(void)
{
	memset(m_Map, 0, sizeof(m_Map)); 
	m_pRenderTarget = nullptr;
	m_TileWidth = 25;

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
	if (indexedPosition.m_PosI == 1)
		return true;
	return false;
}

void CGameMap::Render()
{
	D2D1_ELLIPSE		m_DotEllipse;
	D2D1_RECT_F		connectedLine;
	//D2D1_RECT_F		m_TileVoid;
	D2D1_POINT_2F	m_pos;

	/*
	for(int i = 0; i <= MAX_WIDTH; ++i)
	{
		for(int j = 0; j <= MAX_HEIGHT; ++j)
		{
			switch(m_pInstance->GetMapType(i,j))
			{
			case MO_DOT:
				m_pos.y = 50 + i * 25;
				m_pos.x = 50 + j * 25;
				m_DotEllipse = D2D1::Ellipse( m_pos, 5.0, 5.0 );
				m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pDotBrush);
				break;
			case MO_LINE_UNCONNECTED:
				m_pos.y = 50 + i * 25;
				m_pos.x = 50 + j * 25;
				if(i%2==0)
				{
					connectedLine = D2D1::Rect( m_pos.x - 2.5, m_pos.y - m_TileWidth, m_pos.x + 2.5, m_pos.y + m_TileWidth);
				}
				else
				{
					connectedLine = D2D1::Rect( m_pos.x - m_TileWidth, m_pos.y - 2.5, m_pos.x + m_TileWidth, m_pos.y + 2.5);
				}
				m_pRenderTarget->FillRectangle(connectedLine, m_pUnconnectedLineBrush);
				break;
			case MO_LINE_CONNECTED:
				m_pos.y = 50 + i * 25;
				m_pos.x = 50 + j * 25;
				connectedLine = D2D1::Rect( m_pos.x - 2.5, m_pos.y - 2.5, m_pos.x + 2.5, m_pos.y + 2.5);
				m_pRenderTarget->FillRectangle(connectedLine, m_pConnectedLineBrush);
				break;
			case MO_TILE_VOID:
				m_pos.y = 50 + i * 25;
				m_pos.x = 50 + j * 25;
				connectedLine = D2D1::Rect( m_pos.x - m_TileWidth, m_pos.y - m_TileWidth, m_pos.x + m_TileWidth, m_pos.y + m_TileWidth);
				m_pRenderTarget->FillRectangle(connectedLine, m_pVoidTileBrush);
				break;
			}
		}
	}*/

	//layer : background - tile - line - dot

	//tile
	for (int i = 0; i <= MAX_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_HEIGHT; ++j)
		{
			if (m_pInstance->GetMapType(i,j) == MO_TILE_VOID)
			{
				m_pos.y = -m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i + 1);
				m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j + 1);
				connectedLine = D2D1::Rect( m_pos.x - TILE_SIZE / 2, m_pos.y - TILE_SIZE / 2, m_pos.x + TILE_SIZE / 2, m_pos.y + TILE_SIZE / 2);
				m_pRenderTarget->FillRectangle(connectedLine, m_pVoidTileBrush);
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
				m_pos.y = -m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (i + 1);
				m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * j;
				connectedLine = D2D1::Rect( m_pos.x - LINE_WEIGHT / 2, m_pos.y - TILE_SIZE / 2, m_pos.x + LINE_WEIGHT / 2, m_pos.y + TILE_SIZE / 2);
			}
			else
			{
				m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * i;
				m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * (j + 1);
				connectedLine = D2D1::Rect( m_pos.x - TILE_SIZE / 2, m_pos.y - LINE_WEIGHT / 2, m_pos.x + TILE_SIZE / 2, m_pos.y + LINE_WEIGHT / 2);
			}
			switch (m_pInstance->GetMapType(i,j))
			{
			case MO_LINE_UNCONNECTED:
				m_pRenderTarget->FillRectangle(connectedLine, m_pUnconnectedLineBrush);
				break;
			case MO_LINE_CONNECTED:
				m_pRenderTarget->FillRectangle(connectedLine, m_pConnectedLineBrush);
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
				m_pos.y = m_StartPosition.height + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * i;
				m_pos.x = m_StartPosition.width + ( (LINE_WEIGHT + TILE_SIZE) / 2 ) * j;
				m_DotEllipse = D2D1::Ellipse( m_pos, DOT_RADIUS, DOT_RADIUS );
				m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pDotBrush);
			}
		}
	}
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
	//조심해
	return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ];
}

MapObject CGameMap::GetMapType(int iPos, int jPos)
{
	IndexedPosition tempPos;
	tempPos.m_PosI = iPos;
	tempPos.m_PosJ = jPos;

	return GetMapType(tempPos);
}

bool CGameMap::CreateResource()
{ //SM9: 주기적으로 불리는 Render()안에서 리소스를 생성하는 것은 좋은 방법이 아니다.
	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkMagenta),&m_pDotBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gainsboro),&m_pUnconnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Tomato),&m_pConnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue),&m_pVoidTileBrush);
	}

	//
	return true; //SM9: 이놈은 뭐냠? 보통 CreateResource()같은 함수는 실패할 가능성이 있기 때문에 bool로 반환해라.
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

	m_StartPosition.height = (m_MapSize.m_Height * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT) - LINE_WEIGHT / 2;
	m_StartPosition.width = (m_MapSize.m_Width * (LINE_WEIGHT + TILE_SIZE) + LINE_WEIGHT) - LINE_WEIGHT / 2;
}
