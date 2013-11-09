#pragma once

#include "SceneObject.h"
#include "MacroSet.h"

#define OBJECT_MAX 100

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

	SceneName			getCurrentScene() {return m_SceneStatus;}

protected:
	void RemoveObject();
	
	SceneName m_SceneStatus;
	CSceneObject* m_Object[OBJECT_MAX];

	int m_ObjectCount;

};

