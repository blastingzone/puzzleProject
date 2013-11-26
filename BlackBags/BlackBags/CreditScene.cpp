#include "stdafx.h"
#include "CreditScene.h"
#include "VideoRender.h"
#include "GameData.h"


CCreditScene::CCreditScene(void)
{
	m_SceneStatus = SC_CREDIT;
	m_VideoPath="credit.avi"; //SM9: 이런 상수 리터럴 같은 것들은 나중에 따로 빼도록 할 것
	Init(); //SM9: 이거 상당히 위험한 코드다. 생성자에는 초기화만 하고 뭔가 실행 시키지 말 것.
	//만일, 이 안에서 예외나 에러가 발생한다면 어떻게 할 방법이 없다. 
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
