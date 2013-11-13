#include "stdafx.h"
#include "Scene.h"
#ifdef _DEBUG
#include "FPS.h"
#endif

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
