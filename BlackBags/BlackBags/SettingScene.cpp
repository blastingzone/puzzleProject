#include "stdafx.h"
#include "SettingScene.h"


CSettingScene::CSettingScene(void)
{
	m_SceneStatus = SC_SETTING;
}


CSettingScene::~CSettingScene(void)
{
}

void CSettingScene::EventHandle(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// 조심해!!!!!!!
	// if문 빼야 합니다
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		///////////////////////////////////////////////////////////////////////////
		// 조심해!! 하드코딩으로 데이터 넣어주도록 했습니다
		// 테스트 끝나면 바꿔야합니다
		///////////////////////////////////////////////////////////////////////////
		CGameData::GetInstance()->SetMapSize(8,7);
		CGameData::GetInstance()->SetPlayerNumber(4);
		CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_PLAY );
}

void CSettingScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//아래를 지우고 자동 완성 기능을 구현합니다.
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