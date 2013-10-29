#include "stdafx.h"
#include "GameMap.h"
#include "Renderer.h"

CGameMap* CGameMap::m_pInstance = nullptr;

CGameMap::CGameMap(void)
{
	memset(m_Map, 0, sizeof(MapObject) * MAX_WIDTH * MAX_HEIGHT); //SM9: 이건 sizeof(m_Map) 해도 된다. 컴파일 타임에 크기를 알 수 있기 때문
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

	for(targetRow = 1; targetRow <= m_MapSize.height*2 + 1; ++targetRow){  //SM9: for와 같은 키워드의 경우 괄호를 for ()처럼 키워드 뒤에 한 칸 띄우고 쓸 것.
		
		for(targetColumn = 1; targetColumn <= m_MapSize.width*2 + 1; ++targetColumn) 
		{ //SM9: 웬만하면 시작{와 닫는 }위치를 맞출 것. (VC++쪽에서는 이게 거의 정석)
			if(targetRow % 2 == 1){
				// dot - line - dot - line
				if (targetColumn % 2 == 1){ //SM9: if도 키워드기 때문에 괄호는 한 칸 띄우고..
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
	/*
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
	}*/

	//layer : background - tile - line - dot

	//tile
	for(int i = 0; i <= MAX_WIDTH; ++i)
	{
		for(int j = 0; j <= MAX_HEIGHT; ++j)
		{
			if(m_pInstance->getMapType(i,j) == MO_TILE_VOID){
				m_pos.y = 50 + i * 25;
				m_pos.x = 50 + j * 25;
				connectedLine = D2D1::Rect( m_pos.x - m_tileWidth, m_pos.y - m_tileWidth, m_pos.x + m_tileWidth, m_pos.y + m_tileWidth);
				m_pRenderTarget->FillRectangle(connectedLine, m_pVoidTileBrush);
			}
		}
	}

	//line
	for(int i = 0; i <= MAX_WIDTH; ++i)
	{
		for(int j = 0; j <= MAX_HEIGHT; ++j)
		{
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

			switch(m_pInstance->getMapType(i,j))
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
	for(int i = 0; i <= MAX_WIDTH; ++i)
	{
		for(int j = 0; j <= MAX_HEIGHT; ++j)
		{
			if(m_pInstance->getMapType(i,j) == MO_DOT){
				m_pos.y = 50 + i * 25;
				m_pos.x = 50 + j * 25;
				m_DotEllipse = D2D1::Ellipse( m_pos, 5.0, 5.0 );
				m_pRenderTarget->FillEllipse(&m_DotEllipse, m_pDotBrush);
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

void CGameMap::createResource(){ //SM9: 주기적으로 불리는 Render()안에서 리소스를 생성하는 것은 좋은 방법이 아니다.
	if(m_pRenderTarget == nullptr){
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkMagenta),&m_pDotBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gainsboro),&m_pUnconnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Tomato),&m_pConnectedLineBrush);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AliceBlue),&m_pVoidTileBrush);
	}


	return; //SM9: 이놈은 뭐냠? 보통 CreateResource()같은 함수는 실패할 가능성이 있기 때문에 bool로 반환해라.
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