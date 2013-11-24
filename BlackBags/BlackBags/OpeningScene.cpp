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
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	///////////////////////////////////////////////////////////////////////////
	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
	CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
}

void COpeningScene::MouseOver(Coordinate mouseCoordinate)
{
	//������!!
	//�Ʒ��� ����� �ڵ� �ϼ� ����� �����մϴ�.
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
		//������ ã�� ���ϸ� �׳� ���� ȭ������ �Ѱ� �����ϴ�.
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
}


//��ģ �κ�
void COpeningScene::Render()
{
	if (!CVideoRender::GetInstance()->IsVideoEnd())
	{
		CVideoRender::GetInstance()->RenderVideo();
	}
	else
	{
		//�� �κ� ���� �ʿ�
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
}
