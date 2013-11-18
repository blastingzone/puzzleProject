#include "stdafx.h"
#include "Scene.h"
#ifdef _DEBUG
#include "FPS.h"
#endif
//조심해!! 이 파일도 빼야합니다
#include "GameTimer.h"

CScene::CScene(void)
{
	Init();
}

CScene::~CScene(void)
{

}

void CScene::Render()
{
	for (auto iter: m_Object)
	{
		iter->Render();
#ifdef _DEBUG		
		CFPS::GetInstance()->Update();
		CFPS::GetInstance()->Render();
#endif
	}
	// 조심해!! 이렇게 짜면 안됩니다! 얘들을 PlayScene에 넣을 방법을 생각해보세요!
	if (CScene::GetCurrentScene() == SC_PLAY)
	{
		CGameTimer::GetInstance()->Update();
		CGameTimer::GetInstance()->Render();
	}
}

void CScene::ResizeClient()
{
	for (auto iter: m_Object)
	{
		iter->ResizeClient();
	}
}

void CScene::AddObject( CSceneObject* inputObject )
{
	m_Object.push_back(inputObject);
}

void CScene::RemoveObject()
{
	m_Object.clear();
}

void CScene::Init()
{

}
