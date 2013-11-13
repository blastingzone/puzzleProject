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

	//���� ȭ�� �����Ͽ� ���缭 ������ ������ ������Ʈ ũ�⸦ �����ϵ��� �ϴ� �Լ�
	void ResizeClientSize();

protected:
	CScene* m_CurrentScene;

};

