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
		//������ ã�� ���ϸ� �׳� ���� ȭ������ �Ѱ� �����ϴ�.
		CGameData::GetInstance()->SetCurrentScene( SC_MAIN );
	}
	// ���ο��� ����ó���� �Ǿ��ֽ��ϴ�.
	return true;
}
