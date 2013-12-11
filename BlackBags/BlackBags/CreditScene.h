#pragma once
#include "scene.h"
class CCreditScene :
	public CScene
{
public:
	CCreditScene(void);

	// 조심해!!!
	// 중요! 상속받은 자식의 소멸자에서 자원정리하는 경우는 반드시 virtual 소멸자로 해줘라... 이거 안하면 자식의 소멸자가 불리지 않고 부모의 소멸자가 불린다.
	// 프로젝트 전체적으로 모두 찾아서 virtual 해줄 것. 참고: http://pacs.tistory.com/31

	virtual~CCreditScene(void);

	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void Render();
	virtual bool Init();
	
	virtual void PlayBGM() {}
	virtual void StopBGM() {}

};

