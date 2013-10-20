#include "stdafx.h"
#include "GameMap.h"


CGameMap::CGameMap(void)
{
	m_pInstance = nullptr;
	memset(m_Map, 0, sizeof(CMapObj*) * MAX_WIDTH * MAX_HEIGHT);
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
					m_Map[targetRow][targetColumn] = new CDot();
				} else {
					m_Map[targetRow][targetColumn] = new CLine();
				}
			} else {
				// line - tile - line - tile
				if(targetColumn % 2 == 1){
					m_Map[targetRow][targetColumn] = new CLine();
				} else {
					m_Map[targetRow][targetColumn] = new CTile();
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

bool CGameMap::drawLine(Edge tempEdge){
	int row, column;

	if(tempEdge.direction = VERTICAL){
		row = (2 * tempEdge.row) + 2;
		column = (2 * tempEdge.column) + 1;
	} else {
		row = (2 * tempEdge.row) + 1;
		column = (2 * tempEdge.column) + 2;
	}

	if(isPossible(row, column)){
		m_Map[row][column]->connect();
	} else {
		return false;
	}

	return true;
}