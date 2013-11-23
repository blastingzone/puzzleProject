#include "stdafx.h"
#include "SettingScene.h"


CSettingScene::CSettingScene(void)
{
}


CSettingScene::~CSettingScene(void)
{
}

SceneName CSettingScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		///////////////////////////////////////////////////////////////////////////
		// ������!! �ϵ��ڵ����� ������ �־��ֵ��� �߽��ϴ�
		// �׽�Ʈ ������ �ٲ���մϴ�
		///////////////////////////////////////////////////////////////////////////
		CGameData::GetInstance()->SetMapSize(8,7);
		CGameData::GetInstance()->SetPlayerNumber(4);
		return SC_PLAY;
	}
	return SC_PLAY;
}

void CSettingScene::MouseOver(Coordinate mouseCoordinate)
{
	//������!!
	//�Ʒ��� ����� �ڵ� �ϼ� ����� �����մϴ�.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}

void CSettingScene::Render()
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

void CSettingScene::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();


	}
}