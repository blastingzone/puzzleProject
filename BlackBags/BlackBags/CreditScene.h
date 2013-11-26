#pragma once
#include "scene.h"
class CCreditScene :
	public CScene
{
public:
	CCreditScene(void);
	~CCreditScene(void);

	void EventHandle(Coordinate mouseCoordinate); //SM9: 마찬가지로 virtual 해줘라..
	void MouseOver(Coordinate mouseCoordinate);
	void Render();
	void Init();

private:
	std::string m_VideoPath;
};

