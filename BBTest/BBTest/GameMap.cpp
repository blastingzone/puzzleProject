#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"

CGameMap* CGameMap::m_pInstance = nullptr;

CGameMap::CGameMap(void)
{
	memset(m_Map, 0, sizeof(MapObject) * MAX_WIDTH * MAX_HEIGHT);
	m_pRenderTarget = nullptr;

 	m_Width = 8;
 	m_Height = 7;
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

	for(targetRow = 1; targetRow <= m_Height*2 + 1; ++targetRow){
		for(targetColumn = 1; targetColumn <= m_Width*2 + 1; ++targetColumn){
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
	setSize(5, 5);
	createMap();

	return;
}

bool CGameMap::isPossible(int row, int column){
	return true;
}

void CGameMap::Render()
{
	D2D1_ELLIPSE m_DotEllipse;
	D2D1_RECT_F connectedLine;
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
					connectedLine = D2D1::Rect( m_pos.x - 2.5, m_pos.y - 2.5, m_pos.x + 2.5, m_pos.y + 2.5);
					m_pRenderTarget->FillRectangle(connectedLine, m_pUnconnectedLineBrush);
					break;
				case MO_LINE_CONNECTED:
					m_pos.y = 50 + i * 25;
					m_pos.x = 50 + j * 25;
					connectedLine = D2D1::Rect( m_pos.x - 2.5, m_pos.y - 2.5, m_pos.x + 2.5, m_pos.y + 2.5);
					m_pRenderTarget->FillRectangle(connectedLine, m_pConnectedLineBrush);
					break;
			}
		}
	}
}

bool CGameMap::setSize( int width, int height )
{
	if(m_Width>MAX_WIDTH/2 || m_Height>MAX_HEIGHT/2){
		return false;
	}
	m_Width = width;
	m_Height = height;
	return true;
}

MapObject CGameMap::getMapType(int i, int j)
{
	if(i <= MAX_HEIGHT && j <= MAX_WIDTH)
		return m_Map[i][j];
}

void CGameMap::createResource(){
	if(m_pRenderTarget == nullptr){
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkKhaki),&m_pDotBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red),&m_pUnconnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue),&m_pConnectedLineBrush);
	}

	return;
}

void CGameMap::drawLine(int i, int j){
	m_Map[i][j] = MO_LINE_CONNECTED;
}