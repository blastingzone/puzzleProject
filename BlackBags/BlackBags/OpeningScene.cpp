#include "stdafx.h"
#include "OpeningScene.h"
#include "VideoRender.h"
#include "GameData.h"


COpeningScene::COpeningScene(void)
{
	m_SceneStatus = SC_OPENING;	
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
		//������ ã�� ���ϸ� �׳� ���� ȭ������ �Ѱ� �����ϴ�.
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
		//����� ������ ���� ȭ������ �ѱ�ϴ�.
		Sleep(3*1000);
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
}
