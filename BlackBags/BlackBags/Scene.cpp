#include "stdafx.h"
#include "Scene.h"


CScene::CScene(void)
{

}

CScene::~CScene(void)
{

}

void CScene::Render()
{
	// 함수를 비우고 Virtual로 자식마다 구현하는 방법을 적용해봅니다.
	/*for (auto iter: m_Object)
	{
		iter->Render();
#ifdef _DEBUG		
		CFPS::GetInstance()->EventHandle();
		CFPS::GetInstance()->Render();
#endif
	}*/
	// 조심해!! 이렇게 짜면 안됩니다! 얘들을 PlayScene에 넣을 방법을 생각해보세요!
	//if (CScene::GetCurrentScene() == SC_PLAY)
	//{
	//	CGameTimer::GetInstance()->EventHandle();
	//	CGameTimer::GetInstance()->Render();
	//}
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
