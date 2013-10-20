#include "stdafx.h"
#include "GameMap.h"


CGameMap::CGameMap(void)
{
	m_pInstance = nullptr;
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