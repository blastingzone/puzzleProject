#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"

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
}

CGameMap* CGameMap::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameMap();
	}

	return m_pInstance;
}

void CGameMap::Release()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
	}
	return;
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
	SetMapSize(m_MapSize);
	CreateMap();

	return;
}

bool CGameMap::IsPossible(IndexedPosition indexedPosition)
{
	return true;
}

void CGameMap::Render()
{
	D2D1_ELLIPSE		m_DotEllipse;
	D2D1_RECT_F		connectedLine;
	D2D1_RECT_F		m_TileVoid;
	D2D1_POINT_2F	m_pos;

	CreateResource();
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
				m_pos.y = 50.0f + i * 25.0f;
				m_pos.x = 50.0f + j * 25.0f;
				connectedLine = D2D1::Rect( m_pos.x - m_TileWidth, m_pos.y - m_TileWidth, m_pos.x + m_TileWidth, m_pos.y + m_TileWidth);
				m_pRenderTarget->FillRectangle(connectedLine, m_pVoidTileBrush);
			}
		}
	}

	//line
	for (int i = 0; i <= MAX_WIDTH; ++i)
	{
		for (int j = 0; j <= MAX_HEIGHT; ++j)
		{
			m_pos.y = 50 + i * 25;
			m_pos.x = 50 + j * 25;
			if (i%2==0)
			{
				connectedLine = D2D1::Rect( m_pos.x - 2.5f, m_pos.y - m_TileWidth, m_pos.x + 2.5f, m_pos.y + m_TileWidth);
			}
			else
			{
				connectedLine = D2D1::Rect( m_pos.x - m_TileWidth, m_pos.y - 2.5f, m_pos.x + m_TileWidth, m_pos.y + 2.5f);
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
				m_pos.y = 50 + i * 25;
				m_pos.x = 50 + j * 25;
				m_DotEllipse = D2D1::Ellipse( m_pos, 5.0, 5.0 );
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
}

MapObject CGameMap::GetMapType(int iPos, int jPos)
{
	IndexedPosition tempPos;
	tempPos.m_PosI = iPos;
	tempPos.m_PosJ = jPos;

	return GetMapType(tempPos);
}

bool CGameMap::CreateResource()
{ //SM9: �ֱ������� �Ҹ��� Render()�ȿ��� ���ҽ��� �����ϴ� ���� ���� ����� �ƴϴ�.
	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkMagenta),&m_pDotBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gainsboro),&m_pUnconnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Tomato),&m_pConnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue),&m_pVoidTileBrush);
	}

	//
	return true; //SM9: �̳��� ����? ���� CreateResource()���� �Լ��� ������ ���ɼ��� �ֱ� ������ bool�� ��ȯ�ض�.
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