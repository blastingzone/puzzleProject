#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"

CGameMap* CGameMap::m_pInstance = nullptr;

CGameMap::CGameMap(void)
{
	memset(m_Map, 0, sizeof(MapObject) * MAX_WIDTH * MAX_HEIGHT);
	m_pRenderTarget = nullptr;
	m_tileWidth = 25;

	m_MapSize.width = 5;
	m_MapSize.height = 5;
}


CGameMap::~CGameMap(void)
{
}

CGameMap* CGameMap::GetInstance(){
	if(m_pInstance == nullptr)
		m_pInstance = new CGameMap();

	return m_pInstance;
}

void CGameMap::Release(){
	if(m_pInstance != nullptr)
		delete m_pInstance;

	return;
}

void CGameMap::createMap(){
	int targetRow, targetColumn;

	for(targetRow = 1; targetRow <= m_MapSize.height*2 + 1; ++targetRow){
		for(targetColumn = 1; targetColumn <= m_MapSize.width*2 + 1; ++targetColumn){
			if(targetRow % 2 == 1){
				// dot - line - dot - line
				//m_Map[targetRow][targetColumn] = ((targetColumn % 2 == 1) ? new CDot() : new CLine());
				if(targetColumn % 2 == 1){
					m_Map[targetRow][targetColumn] = MO_DOT;
				} else {
					m_Map[targetRow][targetColumn] = MO_LINE_UNCONNECTED;
				}
			} else {
				// line - tile - line - tile
				if(targetColumn % 2 == 1){
					m_Map[targetRow][targetColumn] = MO_LINE_UNCONNECTED;
				} else {
					m_Map[targetRow][targetColumn] = MO_TILE_VOID;
				}
			}
		}
	}

	return;
}

void CGameMap::init(){
	setMapSize(m_MapSize);
	createMap();

	return;
}

bool CGameMap::isPossible(IndexedPosition indexedPosition){
	return true;
}

void CGameMap::Render()
{
	D2D1_ELLIPSE m_DotEllipse;
	D2D1_RECT_F connectedLine;
	D2D1_RECT_F m_TileVoid;
	D2D1_POINT_2F m_pos;

	createResource();

	for(int i = 0; i <= MAX_WIDTH; ++i)
	{
		for(int j = 0; j <= MAX_HEIGHT; ++j)
		{
			switch(m_pInstance->getMapType(i,j))
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
					connectedLine = D2D1::Rect( m_pos.x - 2.5, m_pos.y - m_tileWidth, m_pos.x + 2.5, m_pos.y + m_tileWidth);
				}
				else
				{
					connectedLine = D2D1::Rect( m_pos.x - m_tileWidth, m_pos.y - 2.5, m_pos.x + m_tileWidth, m_pos.y + 2.5);
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
				connectedLine = D2D1::Rect( m_pos.x - m_tileWidth, m_pos.y - m_tileWidth, m_pos.x + m_tileWidth, m_pos.y + m_tileWidth);
				m_pRenderTarget->FillRectangle(connectedLine, m_pVoidTileBrush);
				break;
			}
		}
	}
}

bool CGameMap::setMapSize(MapSize mapsize)
{
	m_MapSize.width = mapsize.width;
	m_MapSize.height = mapsize.height;
	return true;
}

MapObject CGameMap::getMapType(IndexedPosition indexedPosition)
{
	if(indexedPosition.iPos <= MAX_HEIGHT && indexedPosition.jPos <= MAX_WIDTH)
		return m_Map[indexedPosition.iPos][indexedPosition.jPos];
}

MapObject CGameMap::getMapType(int iPos, int jPos)
{
	IndexedPosition tempPos;
	tempPos.iPos = iPos;
	tempPos.jPos = jPos;

	return getMapType(tempPos);
}

void CGameMap::createResource(){
	if(m_pRenderTarget == nullptr){
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkKhaki),&m_pDotBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red),&m_pUnconnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue),&m_pConnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow),&m_pVoidTileBrush);
	}

	return;
}

void CGameMap::drawLine(IndexedPosition indexedPosition){
	m_Map[indexedPosition.iPos][indexedPosition.jPos] = MO_LINE_CONNECTED;
}
void CGameMap::drawLine(int iPos, int jPos){
	IndexedPosition indexedPosition;
	indexedPosition.iPos=iPos;
	indexedPosition.jPos =jPos;

	drawLine(indexedPosition);
}