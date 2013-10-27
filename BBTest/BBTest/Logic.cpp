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
	if(m_pInstance == nullptr)
		m_pInstance = new CLogic();

	return m_pInstance;
}

void CLogic::Release()
{
	if(m_pInstance != nullptr)
		delete m_pInstance;

	return;
}

//Logic���� �ʱ�ȭ �Լ�
void CLogic::init()
{

}

//���� ���� ������ ������Ʈ ���ִ� �Լ�
void CLogic::update( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	indexedPosition = calcualteIndex(mouseCoordinate);

	//isPossible üũ �Ŀ� gameMap ȣ���ؼ� �ݿ�
}

//���콺 ��ǥ���� index�� �ٲٴ� �Լ�
IndexedPosition CLogic::calcualteIndex( Coordinate mouseCoordinate )
{
	IndexedPosition indexedPosition;
	indexedPosition.iPos = 0;
	indexedPosition.jPos = 0;

	return indexedPosition;
}
