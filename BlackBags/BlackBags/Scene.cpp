#include "stdafx.h"
#include "Scene.h"


CScene::CScene(void)
{
	m_ObjectCount = 0;
}

CScene::~CScene(void)
{
}

void CScene::Render()
{
	for (int i = 0 ; i < m_ObjectCount ; ++i )
	{
		m_Object[i]->Render();
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
		delete( m_Object[i] );
	}
}
