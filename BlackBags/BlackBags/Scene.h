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
	void				RemoveObject();

	SceneName			getCurrentScene() {return m_SceneStatus;}

protected:
	
	SceneName m_SceneStatus;
	CSceneObject* m_Object[OBJECT_MAX]; //SM9: 이런거는 std::list나 std::vector로 관리하는게 낫다. 배열로 관리할 경우 범위 벗어나면 위험한 경우가 발생

	int m_ObjectCount;

};

