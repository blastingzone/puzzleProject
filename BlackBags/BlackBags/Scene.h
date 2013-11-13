#pragma once

#include "SceneObject.h"
#include "MacroSet.h"
#include <vector>

enum SceneName
{
	SC_OPENING,
	SC_MAIN,
	SC_SETTING,
	SC_PLAY,
	SC_RESULT,
	SC_CREDIT,
	SC_NOSCENE
};

class CScene
{
public:
	CScene(void);
	virtual~CScene(void);

	void				Render();
	virtual SceneName	Update(Coordinate mouseCoordinate) = 0;
	void				AddObject(CSceneObject* inputObject);
	void				ResizeClient();
	void				RemoveObject();
	void				Init();

	SceneName			GetCurrentScene() {return m_SceneStatus;}

protected:
	
	SceneName								m_SceneStatus;
	std::vector<CSceneObject*>				m_Object;
	std::vector<CSceneObject*>::iterator	m_Iter;
};

