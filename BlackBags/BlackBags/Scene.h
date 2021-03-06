﻿#pragma once

#include "SceneObject.h"
#include <vector>
#include "Renderer.h"
#ifdef _DEBUG
#include "FPS.h"
#endif
class CScene
{
public:
	CScene(void);
	virtual~CScene(void);

	virtual bool	Init() = 0;
	virtual void	Render();

	/*	마우스 클릭 시 호출 되는 함수로써 입력 값을 바탕으로 현재 scene 상태 업데이트 */
	virtual void	EventHandle(Coordinate mouseCoordinate) = 0;
	virtual void	MouseOver(Coordinate mouseCoordinate) = 0;
	virtual void	PlayBGM() = 0;
	virtual void	StopBGM() = 0;

	virtual void	ResizeClient();
	void			AddObject(CSceneObject* inputObject);
	void			RemoveObject();
	
	SceneName		GetCurrentScene() {return m_SceneStatus;}

protected:
	SceneName								m_SceneStatus;
	std::vector<CSceneObject*>				m_Object;
	std::vector<CSceneObject*>::iterator	m_Iter;
};

