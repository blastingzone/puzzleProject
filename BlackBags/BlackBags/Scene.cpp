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
	if (!m_Object.empty() )
	{
		m_Iter = m_Object.begin();
		while(m_Iter!=m_Object.end())
		{
			(*m_Iter)->Render();
			++m_Iter;
			#ifdef _DEBUG		
			CFPS::GetInstance()->Update();
			CFPS::GetInstance()->Render();
			#endif
		}
	}
}

void CScene::ResizeClient()
{
	if (!m_Object.empty() )
	{
			for (m_Iter = m_Object.begin(); m_Iter != m_Object.end(); ++m_Iter);
			{
				(*m_Iter)->ResizeClient();
			}
	}
}

void CScene::AddObject( CSceneObject* inputObject )
{
	m_Object.push_back(inputObject);
}

void CScene::RemoveObject()
{
// 	if (!m_Object.empty() )
// 	{
// 		for (m_Iter = m_Object.begin(); m_Iter != m_Object.end(); ++m_Iter)
// 		{
// 			delete (*m_Iter);
// 		}
// 	}
// 	m_Object.clear();
}

void CScene::Init()
{
	// √ ±‚»≠ §§§§

}
