#pragma once
#include "d2d1.h"

#define MAX_WIDTH	21
#define MAX_HEIGHT	21

struct IndexedPosition
{
	int m_PosI;
	int m_PosJ;
};
struct Coordinate
{
	int m_PosX;
	int m_PosY;
};
struct MapSize
{
	int m_Width;
	int m_Height;
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

const float TILE_SIZE = 80.0f;
const float LINE_WEIGHT = 10.0f;
const float DOT_RADIUS = 6.0f;

class CGameMap
{
public:
	CGameMap(void);
	~CGameMap(void);

	void				Init();
	static CGameMap*	GetInstance();
	static bool			ReleaseInstance();
	bool				Release();

	void				Render();

	bool SetMapSize(MapSize mapsize);
	void DrawLine(IndexedPosition indexedPosition);
	void DrawLine(int iPos, int jPos);
	
	bool		IsPossible(IndexedPosition indexedPosition);
	D2D1_SIZE_F GetStartPosition();

	MapObject	GetMapType(IndexedPosition indexedPosition);
	MapObject	GetMapType(int iPos, int jPos);

	void SetMapType(IndexedPosition indexedPosition, MapObject newMapObject);
	void SetMapType(int iPos, int jPos, MapObject newMapObject);

private: 
	void CreateMap();
	bool CreateResource();
	void CalcStartPosition();

	MapSize		m_MapSize;
	D2D1_SIZE_F	m_StartPosition;

	static CGameMap*	m_pInstance; //singleton instance

	//renderer를 위한 임시 배열입니다. 
	MapObject	m_Map[MAX_WIDTH][MAX_HEIGHT];

	ID2D1HwndRenderTarget*	m_pRenderTarget;

	ID2D1SolidColorBrush*	m_pDotBrush;
	ID2D1SolidColorBrush*	m_pUnconnectedLineBrush;
	ID2D1SolidColorBrush*	m_pConnectedLineBrush;
	ID2D1SolidColorBrush*	m_pPossibleLineBrush;
	ID2D1SolidColorBrush*	m_pTileBrush;
	ID2D1SolidColorBrush*	m_pVoidTileBrush;

	D2D1_ELLIPSE			m_DotEllipse;
};

