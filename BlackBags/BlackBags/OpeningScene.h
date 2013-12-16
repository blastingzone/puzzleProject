#pragma once
#include "Scene.h"
#include "BandiVideoLibrary.h"
#include "BandiVideoTexture_DX9.h"

class COpeningScene :
	public CScene
{
public:
	COpeningScene(void);
	virtual ~COpeningScene(void);

	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void Render();
	
	virtual void PlayBGM() {}
	virtual void StopBGM() {}

	virtual bool Init();

};
