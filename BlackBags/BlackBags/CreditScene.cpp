#include "stdafx.h"
#include "CreditScene.h"


CCreditScene::CCreditScene(void)
{
}


CCreditScene::~CCreditScene(void)
{
}

void CCreditScene::EventHandle(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// !!!!!!!
	// if�� ���� �մϴ�
	// ���� �ٱ��� return ����
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
}

void CCreditScene::MouseOver(Coordinate mouseCoordinate)
{
	//������!!
	//�Ʒ��� ����� �ڵ� �ϼ� ����� �����մϴ�.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}

void CCreditScene::Render()
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