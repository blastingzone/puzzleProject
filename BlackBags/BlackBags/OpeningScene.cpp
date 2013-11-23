#include "stdafx.h"
#include "OpeningScene.h"
#include "VideoRender.h"


COpeningScene::COpeningScene(void)
{
	m_SceneStatus = SC_OPENING;	
	Init();
}

COpeningScene::~COpeningScene(void)
{
	//CVideoRender::GetInstance()->CleanUp();
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
	CVideoRender::GetInstance()->StartVideo();
}

void COpeningScene::RenderVideo()
{
	if (!CVideoRender::GetInstance()->IsVideoEnd())
	{
		CVideoRender::GetInstance()->RenderVideo();
	}
	else
	{
		//다음 씬으로 전환해 버리기
	}
}
