#include "stdafx.h"
#include "GameMap.h"

CGameMap* CGameMap::m_pInstance = nullptr;

CGameMap::CGameMap(void)
{
	memset(m_Map, 0, sizeof(MapObject) * MAX_WIDTH * MAX_HEIGHT);
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
	createMap();

	return;
}

bool CGameMap::isPossible(int row, int column){
	//일단 자세한 설명은 생략한다.
	return true;
}

void CGameMap::Render(ID2D1HwndRenderTarget* target)
{
	ID2D1SolidColorBrush* m_pDotBrush;
	D2D1_ELLIPSE m_DotEllipse;

	D2D1_POINT_2F m_pos;

	target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkKhaki),&m_pDotBrush);
	for(int i = 0; i <= MAX_WIDTH; ++i)
	{
		for(int j = 0; j <= MAX_HEIGHT; ++j)
		{
			m_pos.y = i * 30;
			m_pos.x = j * 30;
			m_DotEllipse = D2D1::Ellipse( m_pos, 5, 5 );
			target->FillEllipse(&m_DotEllipse, m_pDotBrush);
		}
	}
}