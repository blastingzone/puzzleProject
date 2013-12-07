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
