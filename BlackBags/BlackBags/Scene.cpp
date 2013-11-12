#include "stdafx.h"
#include "Scene.h"
#ifdef _DEBUG
#include "FPS.h"
#endif

CScene::CScene(void)
{
	m_ObjectCount = 0;
	Init();
}

CScene::~CScene(void)
{
	
}

void CScene::Render()
{
	for (int i = 0 ; i < m_ObjectCount ; ++i )
	{
		m_Object[i]->Render();
#ifdef _DEBUG		
		CFPS::GetInstance()->Update();
		CFPS::GetInstance()->Render();
#endif
	}
}

void CScene::ResizeClient()
{
	for (int i = 0 ; i < m_ObjectCount ; ++i )
	{
		m_Object[i]->ResizeClient();
	}
}

void CScene::AddObject( CSceneObject* inputObject )
{
	if (m_ObjectCount < OBJECT_MAX)
	{
		m_Object[m_ObjectCount++] = inputObject;
	}
}

void CScene::RemoveObject()
{
	for (int i = 0 ; i < m_ObjectCount ; ++i )
	{
		if (m_Object[i])
		{
			m_Object[i] = nullptr;
		}
	}
}

void CScene::Init()
{
	for (int i = 0 ; i < OBJECT_MAX ; ++i )
		{
			m_Object[i] = nullptr;
		}
}
