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
	CVideoRender::GetInstance()->ReleaseInstance();
}

SceneName COpeningScene::Update(Coordinate mouseCoordinate)
{
	///////////////////////////////////////////////////////////////////////////
	// ������!!!!!!!
	// if�� ���� �մϴ�
	///////////////////////////////////////////////////////////////////////////

	//SM9: ���� �ǹ̰� �ִ� �ڵ��ΰ�?
	if (mouseCoordinate.m_PosX > 0)
		return SC_MAIN;
	return SC_MAIN;
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
