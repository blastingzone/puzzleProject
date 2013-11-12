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
	//SM9: �̰� �����ϴ�... OBJECT_MAX ������ ��� ���� ó�� �Ұ�? �׳� �߱�?
	// �� ��� assert�� ����ϴ� ���� �� ������
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

	//SM9: �̰� �ǹ̰� �ִ� �ڵ��ΰ�?
	//�� �ڵ�� ��ġ: memset(m_Object, NULL, sizeof(m_Object)) ;
}
