#include "stdafx.h"
#include "ResultScene.h"


CResultScene::CResultScene(void)
{
}


CResultScene::~CResultScene(void)
{
}

SceneName CResultScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	///////////////////////////////////////////////////////////////////////////
	if(mouseCoordinate.m_PosX > 0)
		return SC_OPENING;
	return SC_OPENING;
}

void CResultScene::MouseOver(Coordinate mouseCoordinate)
{
	//������!!
	//�Ʒ��� ����� �ڵ� �ϼ� ����� �����մϴ�.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}