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
	void Release();

	//현재 화면 스케일에 맞춰서 각각의 씬들의 오브젝트 크기를 변경하도록 하는 함수
	void ResizeClientSize();

protected:
	CScene* m_CurrentScene;

};

