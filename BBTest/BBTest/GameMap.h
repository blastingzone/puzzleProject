#pragma once
#include "d2d1.h"

#define MAX_WIDTH	21
#define MAX_HEIGHT	21

enum MapObject
{
	MO_SENTINEL,
	MO_DOT = 10,
	MO_LINE_UNCONNECTED = 20,
	MO_LINE_CONNECTED,
	MO_LINE_HIDDEN,
	MO_TILE_VOID = 30,
	MO_TILE_GOLD,
	MO_TILE_TRASH,
	MO_TILE_VOID_P1 = 100,
	MO_TILE_GOLD_P1,
	MO_TILE_TRASH_P1,
	MO_TILE_VOID_P2 = 200,
	MO_TILE_GOLD_P2,
	MO_TILE_TRASH_P2,
	MO_TILE_VOID_P3 = 300,
	MO_TILE_GOLD_P3,
	MO_TILE_TRASH_P3,
	MO_TILE_VOID_P4 = 400,
	MO_TILE_GOLD_P4,
	MO_TILE_TRASH_P4
};

class CGameMap
{
public:
	CGameMap(void);
	~CGameMap(void);

	void init();
	//bool drawLine(Edge tempEdge); //success : return true
	static CGameMap*	GetInstance();
	static void			Release();
	static void			Render(ID2D1HwndRenderTarget* target);
	bool setSize(int width, int height);

protected:
	int m_Width;
	int m_Height;

	static CGameMap*	m_pInstance; //singleton instance
	
	//renderer를 위한 임시 배열입니다. 하하하!
	MapObject			m_Map[MAX_WIDTH][MAX_HEIGHT];
	MapObject			getMapType(int i, int j);

	void createMap();
	bool isPossible(int row, int column);

};

