#pragma once
#include "GameData.h"

class CSceneObject
{
public:
	CSceneObject(void);
	virtual ~CSceneObject(void);

	virtual void Render() = 0;
	virtual void ResizeClient() = 0;
	virtual void SetObjectSize() = 0;
};

