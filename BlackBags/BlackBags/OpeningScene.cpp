#include "stdafx.h"
#include "OpeningScene.h"
#include "VideoRender.h"
#include "GameData.h"


COpeningScene::COpeningScene(void)
{
	m_SceneStatus = SC_OPENING;	
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

bool COpeningScene::Init()
{
	if (CVideoRender::GetInstance()->CreateLibrary(_OPENING_MOVIE) )
	{
		CVideoRender::GetInstance()->StartVideo();
	}
	else
	{
		//������ ã�� ���ϸ� �׳� ���� ȭ������ �Ѱ� �����ϴ�.
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}

	return true;
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