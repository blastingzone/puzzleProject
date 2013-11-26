#pragma once
#include "Scene.h"
#include "BandiVideoLibrary.h"
#include "BandiVideoTexture_DX9.h"

class COpeningScene :
	public CScene
{
public:
	COpeningScene(void);
	~COpeningScene(void);

	void EventHandle(Coordinate mouseCoordinate); //SM9: 상속받아서 오버라이딩 하는데.. 왜 virtual 선언 안해주니? ㅋㅋ 
	void MouseOver(Coordinate mouseCoordinate);
	void Render();

	void Init();

private:
	std::string m_VideoPath;

};
