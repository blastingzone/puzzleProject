#pragma once

#include "Scene.h"

class CSceneManager
{
public:
	CSceneManager(void);
	~CSceneManager(void);

	void SetNextScene();
	void CreateScene();
	void Render() {m_CurrentScene->Render();}

	/*	마우스 클릭 메시지가 발생하면 호출되어서 m_CurrentScene을 update한다.
		만약 m_CurrentScene이 종료되어야 하는 상황이면 현재의 scene을 해제하고 다음 scene을 생성한다. */
	void Update(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);

	/*	현재 화면 스케일에 맞춰서 각각의 씬들의 오브젝트 크기를 변경하도록 하는 함수 */
	void ResizeClientSize();
	void ChangeScene(const SceneName& newScene);

protected:
	

	CScene* m_CurrentScene;
};

