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
		//���� ������ ��ȯ�� ������
	}
}
