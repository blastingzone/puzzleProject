#include "stdafx.h"
#include "Logic.h"

CLogic* CLogic::m_pInstance = nullptr;
CLogic::CLogic(void)
{
}


CLogic::~CLogic(void)
{
}

CLogic* CLogic::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLogic();
	}

	return m_pInstance;
}

void CLogic::Release()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
	}
	return;
}

//Logic관련 초기화 함수
void CLogic::Init()
{

}

//지도 관련 정보를 업데이트 해주는 함수
void CLogic::Update( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	indexedPosition = CalcualteIndex(mouseCoordinate);

	//IsPossible 체크 후에 gameMap 호출해서 반영
}

//마우스 좌표값을 index로 바꾸는 함수
IndexedPosition CLogic::CalcualteIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	indexedPosition.m_PosI = 0;
	indexedPosition.m_PosJ = 0;

	return indexedPosition;
}
