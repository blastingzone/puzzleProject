#include "stdafx.h"
#include "CreditScene.h"
#include "VideoRender.h"
#include "GameData.h"


CCreditScene::CCreditScene(void)
{
	m_SceneStatus = SC_CREDIT;
}


CCreditScene::~CCreditScene(void)
{
	CVideoRender::GetInstance()->CleanUp();
}

void CCreditScene::EventHandle(Coordinate mouseCoordinate)
{
	CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
}

void CCreditScene::MouseOver(Coordinate mouseCoordinate)
{

}

void CCreditScene::Render()
{
	if (!CVideoRender::GetInstance()->IsVideoEnd())
	{
		CVideoRender::GetInstance()->RenderVideo();
	}
	else
	{
		CGameData::GetInstance()->SetCurrentScene( SC_OPENING );
	}
}

bool CCreditScene::Init()
{
	if (CVideoRender::GetInstance()->CreateLibrary(_CREDIT_MOVIE))
	{
		CVideoRender::GetInstance()->StartVideo();
	}
	else
	{
		//비디오를 찾지 못하면 그냥 메인 화면으로 넘겨 버립니다.
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
	// 내부에서 예외처리가 되어있습니다.
	return true;
}
