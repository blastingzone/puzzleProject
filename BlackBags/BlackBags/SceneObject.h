#pragma once
class CSceneObject
{
public:
	CSceneObject(void);
	virtual ~CSceneObject(void);

	virtual void Render() = 0;
	virtual void ResizeClient() = 0;
};

