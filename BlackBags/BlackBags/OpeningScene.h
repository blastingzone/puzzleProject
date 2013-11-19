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

	SceneName Update(Coordinate mouseCoordinate);
	void MouseOver(Coordinate mouseCoordinate);
	void RenderVideo();

	void Init();

};
