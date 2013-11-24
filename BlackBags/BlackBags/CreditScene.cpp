#include "stdafx.h"
#include "CreditScene.h"
#include "VideoRender.h"
#include "GameData.h"


CCreditScene::CCreditScene(void)
{
	m_SceneStatus = SC_CREDIT;
	m_VideoPath="credit.avi";
	Init();
}


CCreditScene::~CCreditScene(void)
{
	CVideoRender::GetInstance()->CleanUp();
}

void CCreditScene::EventHandle(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// !!!!!!!
	// if문 빼야 합니다
	// 제일 바깥쪽 return 도요
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
}

void CCreditScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//아래를 지우고 자동 완성 기능을 구현합니다.
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}

void CCreditScene::Render()
{
	if (!CVideoRender::GetInstance()->IsVideoEnd())
	{
		CVideoRender::GetInstance()->RenderVideo();
	}
	else
	{
		//이 부분 수정 필요
		CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
	}
}

void CCreditScene::Init()
{
	if (CVideoRender::GetInstance()->CreateLibrary(m_VideoPath))
	{
		CVideoRender::GetInstance()->StartVideo();
	}
	else
	{
		//비디오를 찾지 못하면 그냥 메인 화면으로 넘겨 버립니다.
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
}
