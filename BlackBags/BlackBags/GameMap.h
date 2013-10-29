#pragma once
#include "d2d1.h"

#define MAX_WIDTH	21
#define MAX_HEIGHT	21

struct IndexedPosition
{
	int iPos;
	int jPos;
};
struct Coordinate
{
	int xPos;
	int yPos;
};
struct MapSize
{
	int width;
	int height;
};

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
	void				Render();


	bool setMapSize(MapSize mapsize);
	void drawLine(IndexedPosition indexedPosition);
	void drawLine(int iPos, int jPos);


protected: //SM9: �̰� ��� ��� �޴°� �ƴ϶�� private�� �ض�.

	//int m_Width;
	//int m_Height;
	MapSize m_MapSize;
	double m_tileWidth;


	static CGameMap*	m_pInstance; //singleton instance

	//renderer�� ���� �ӽ� �迭�Դϴ�. ������!
	MapObject	m_Map[MAX_WIDTH][MAX_HEIGHT];
	MapObject	getMapType(IndexedPosition indexedPosition);
	MapObject  getMapType(int xPos, int jPos);

	D2D1_ELLIPSE m_DotEllipse;

	void createMap();
	bool isPossible(IndexedPosition indexedPosition);

	ID2D1HwndRenderTarget*	m_pRenderTarget;

	ID2D1SolidColorBrush*	m_pDotBrush;
	ID2D1SolidColorBrush*	m_pUnconnectedLineBrush;
	ID2D1SolidColorBrush*	m_pConnectedLineBrush;
	ID2D1SolidColorBrush*	m_pPossibleLineBrush;
	ID2D1SolidColorBrush*	m_pTileBrush;
	ID2D1SolidColorBrush*	m_pVoidTileBrush;

	void createResource();
};

