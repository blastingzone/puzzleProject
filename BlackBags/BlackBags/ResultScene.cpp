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

void CResultScene::TimeOut()
{
}

void CResultScene::Render()
{
	for (auto iter: m_Object)
	{
		iter->Render();
#ifdef _DEBUG		
		CFPS::GetInstance()->Update();
		CFPS::GetInstance()->Render();
#endif
	}
}