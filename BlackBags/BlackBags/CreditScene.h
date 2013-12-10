#pragma once
#include "scene.h"
class CCreditScene :
	public CScene
{
public:
	CCreditScene(void);
	virtual ~CCreditScene(void); //sm9: �߿�! ��ӹ��� �ڽ��� �Ҹ��ڿ��� �ڿ������ϴ� ���� �ݵ�� virtual �Ҹ��ڷ� �����... �̰� ���ϸ� �ڽ��� �Ҹ��ڰ� �Ҹ��� �ʰ� �θ��� �Ҹ��ڰ� �Ҹ���.
	//������Ʈ ��ü������ ��� ã�Ƽ� virtual ���� ��. ����: http://pacs.tistory.com/31

	virtual void EventHandle(Coordinate mouseCoordinate);
	virtual void MouseOver(Coordinate mouseCoordinate);
	virtual void Render();
	virtual bool Init();
	
	virtual void PlayBGM() {}
	virtual void StopBGM() {}

};

