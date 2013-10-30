#pragma once
#include "GameMap.h"

class CLogic
{
public:
	CLogic(void);
	~CLogic(void);

	void					Init();
	static CLogic*		GetInstance();
	static void			Release();

	void StartGame();
	void ShowResult();
	void CheckScore();

	void Update(Coordinate mouseCoordinate);
	bool DrawLine(IndexedPosition indexedPosition);

	IndexedPosition CalcualteIndex(Coordinate mouseCoordinate);

	bool IsEnd();

	//�ʻ������ ��ǻ� ��ǥ�� Ŭ���ϴ� ���� ���ٵ� ��� �޾Ƽ� �Ѱ��� ���ΰ�?
	void GetMapSize();

private:

	static CLogic*	m_pInstance; //singleton instance
};

