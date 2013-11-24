#include "stdafx.h"
#include "OpeningScene.h"
#include "VideoRender.h"
#include "GameData.h"


COpeningScene::COpeningScene(void)
{
	m_SceneStatus = SC_OPENING;	
	m_VideoPath = "intro.avi";
	Init();
}

COpeningScene::~COpeningScene(void)
{
	CVideoRender::GetInstance()->StopVideo();
}

void COpeningScene::EventHandle(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// 조심해!!!!!!!
	// if문 빼야 합니다
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
}

void COpeningScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//아래를 지우고 자동 완성 기능을 구현합니다.
	if (mouseCoordinate.m_PosX > 0)
	{
		//
	}
}

void COpeningScene::Init()
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


//고친 부분
void COpeningScene::Render()
{
	if (!CVideoRender::GetInstance()->IsVideoEnd())
	{
		CVideoRender::GetInstance()->RenderVideo();
	}
	else
	{
		//이 부분 수정 필요
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
}
