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

void CScene::ResizeClient()
{
	for (int i = 0 ; i < m_ObjectCount ; ++i )
	{
		m_Object[i]->ResizeClient();
	}
}

void CScene::AddObject( CSceneObject* inputObject )
{
	//SM9: 이거 위험하닷... OBJECT_MAX 넘으면 어떻게 에러 처리 할겨? 그냥 쌩깜?
	// 이 경우 assert를 사용하는 것이 더 적절함
	if (m_ObjectCount < OBJECT_MAX)
	{
		m_Object[m_ObjectCount++] = inputObject;
	}
}

void CScene::RemoveObject()
{
	for (int i = 0 ; i < OBJECT_MAX ; ++i )
	{
		if (m_Object[i])
		{
			m_Object[i] = NULL;
		}
	}

	//SM9: 이게 의미가 있는 코드인가?
	//이 코드와 동치: memset(m_Object, NULL, sizeof(m_Object)) ;
}
