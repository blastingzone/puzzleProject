#include "stdafx.h"
#include "OpeningScene.h"
#include "VideoRender.h"
#include "GameData.h"


COpeningScene::COpeningScene(void)
{
	m_SceneStatus = SC_OPENING;	
	//조심해!!
	//config.h에 넣어주고.. Init을 다른데서 해버렷.
	m_VideoPath = "opening.avi";
	Init();
}

COpeningScene::~COpeningScene(void)
{
	CVideoRender::GetInstance()->CleanUp();
}

void COpeningScene::EventHandle(Coordinate mouseCoordinate)
{
	CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
}

void COpeningScene::MouseOver(Coordinate mouseCoordinate)
{
	
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


void COpeningScene::Render()
{
	if (!CVideoRender::GetInstance()->IsVideoEnd())
	{
		CVideoRender::GetInstance()->RenderVideo();
	}
	else
	{
		//재생이 끝나면 다음 화면으로 넘깁니다.
		Sleep(3*1000);
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
}
