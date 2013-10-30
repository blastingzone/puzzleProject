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

//Logic���� �ʱ�ȭ �Լ�
void CLogic::Init()
{

}

//���� ���� ������ ������Ʈ ���ִ� �Լ�
void CLogic::Update( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	indexedPosition = CalcualteIndex(mouseCoordinate);

	//IsPossible üũ �Ŀ� gameMap ȣ���ؼ� �ݿ�
}

//���콺 ��ǥ���� index�� �ٲٴ� �Լ�
IndexedPosition CLogic::CalcualteIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	//����//////////////////////////
	mouseCoordinate.m_PosX = 0;

	indexedPosition.m_PosI = 0;
	indexedPosition.m_PosJ = 0;

	return indexedPosition;
}
