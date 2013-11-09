#pragma once

#include "Scene.h"
#include "MacroSet.h"


class CSceneManager
{
public:
	CSceneManager(void);
	~CSceneManager(void);

	void SetNextScene();
	void CreateScene();
	void Render() {m_CurrentScene->Render();}
	void Update(Coordinate mouseCoordinate);

protected:
	CScene* m_CurrentScene;

};

