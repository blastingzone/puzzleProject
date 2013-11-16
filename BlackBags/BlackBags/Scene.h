#pragma once

#include "SceneObject.h"
#include <vector>
#include "GameData.h"'
#include "Renderer.h"

class CScene
{
public:
	CScene(void);
	virtual~CScene(void);

	void				Init();
	void				Render();

	/*	마우스 클릭 시 호출 되는 함수로써 입력 값을 바탕으로 현재 scene 상태 업데이트 */
	virtual SceneName	Update(Coordinate mouseCoordinate) = 0;
	void				ResizeClient();
	void				AddObject(CSceneObject* inputObject);
	void				RemoveObject();
	
	SceneName			GetCurrentScene() {return m_SceneStatus;}

protected:
	SceneName								m_SceneStatus;
	std::vector<CSceneObject*>				m_Object;
	std::vector<CSceneObject*>::iterator	m_Iter;
};

