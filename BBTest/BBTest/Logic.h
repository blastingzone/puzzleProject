#pragma once
#include "GameMap.h"

class CLogic
{
public:
	CLogic(void);
	~CLogic(void);



	void init();
	static CLogic*	GetInstance();
	static void			Release();

	void startGame();
	void showResult();
	void checkScore();

	void update(Coordinate mouseCoordinate);
	bool drawLine(IndexedPosition indexedPosition);

	IndexedPosition calcualteIndex(Coordinate mouseCoordinate);

	bool isEnd();


	//�ʻ������ ��ǻ� ��ǥ�� Ŭ���ϴ� ���� ���ٵ� ��� �޾Ƽ� �Ѱ��� ���ΰ�?
	void getMapSize();





protected:

	static CLogic*	m_pInstance; //singleton instance
};

