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


	//맵사이즈는 사실상 좌표를 클릭하는 것이 될텐데 어떻게 받아서 넘겨줄 것인가?
	void getMapSize();





protected:

	static CLogic*	m_pInstance; //singleton instance
};

