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

	void EventHandle(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
	void Render();

	void Init();

private:
	std::string m_VideoPath;

};
