#include "stdafx.h"
#include "CreditScene.h"


CCreditScene::CCreditScene(void)
{
}


CCreditScene::~CCreditScene(void)
{
}

SceneName CCreditScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	// ���� �ٱ��� return ����
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_OPENING;
	return SC_OPENING;
}