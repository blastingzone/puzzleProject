#pragma once
#include "d2d1.h"

#define MAX_WIDTH	21
#define MAX_HEIGHT	21

struct IndexedPosition
{
	IndexedPosition() : m_PosI(0), m_PosJ(0) {} //SM9: 벡터 같은 STL 컨테이너 안에서 쓸거면 이런식으로 반드시 데이터를 초기화해주는 생성자를 만드는 습관을 들일 것.
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
	static CGameMap*	GetInstance(); //SM9: 이 프로젝트에서는 굳이 싱글톤을 쓸 이유가 없음. 어차피 전역 객체처럼 쓰고 있는 상황
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

	ID2D1SolidColorBrush*	m_pTileP1;
	ID2D1SolidColorBrush*	m_pTileP2;
	ID2D1SolidColorBrush*	m_pTileP3;
	ID2D1SolidColorBrush*	m_pTileP4;

	D2D1_ELLIPSE			m_DotEllipse;
};

