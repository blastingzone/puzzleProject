#pragma once
#include "d2d1.h"

#define MAX_WIDTH	21
#define MAX_HEIGHT	21

struct IndexedPosition
{
	IndexedPosition() : m_PosI(0), m_PosJ(0) {}

	IndexedPosition(const IndexedPosition& inputIndexedPosition)
	{
		m_PosI = inputIndexedPosition.m_PosI;
		m_PosJ = inputIndexedPosition.m_PosJ;
	}

	IndexedPosition(int PosI, int PosJ)
	{
		m_PosI = PosI;
		m_PosJ = PosJ;
	}

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

enum MO_TYPE
{
	MO_SENTINEL,
	MO_DOT = 10,
	MO_LINE_UNCONNECTED = 20,
	MO_LINE_CONNECTED,
	MO_LINE_HIDDEN,
	MO_TILE = 30,
};

enum MO_OWNER
{
	MO_NOBODY = -1,
	MO_PLAYER1,
	MO_PLAYER2,
	MO_PLAYER3,
	MO_PLAYER4
};

enum MO_ITEM
{
	MO_NOTHING,
	MO_GOLD,
	MO_TRASH
};

struct MapObject
{
	MapObject () : m_Type(MO_SENTINEL), m_Owner(MO_NOBODY), m_Item(MO_NOTHING), m_Flag(false) {}

	MO_TYPE		m_Type;
	MO_OWNER	m_Owner;
	MO_ITEM		m_Item;
	
	bool m_Flag;
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
	//static CGameMap*	GetInstance();
	//bool				ReleaseInstance();
	bool				Release();

	void				Render();

	bool SetMapSize(MapSize mapsize);
	void DrawLine(const IndexedPosition& indexedPosition);
	void DrawLine(const int& iPos, const int& jPos);
	
	void DeleteLine(const IndexedPosition& indexedPosition);
	void DeleteLine(const int& iPos, const int& jPos);

	D2D1_SIZE_F GetStartPosition();

	MO_TYPE	GetMapType(IndexedPosition indexedPosition);
	MO_TYPE	GetMapType(const int& iPos, const int& jPos);

	MO_OWNER GetMapOwner(IndexedPosition indexedPosition);
	MO_OWNER GetMapOwner(const int& iPos, const int& jPos);

	void SetMapOwner(IndexedPosition indexedPosition, MO_OWNER owner);
	void SetMapOwner(const int& iPos, const int& jPos, MO_OWNER owner);

	void SetItem(IndexedPosition indexedPosition, MO_ITEM item);
	void SetItem(const int& iPos, const int& jPos, MO_ITEM item);

	bool GetMapFlag(IndexedPosition indexedPosition){return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Flag;}
	void SetMapFlag(IndexedPosition indexedPosition,bool flag);

private: 
	void CreateMap();
	bool CreateResource();
	void CalcStartPosition();

	MapSize		m_MapSize;
	D2D1_SIZE_F	m_StartPosition;

	//static CGameMap*	m_pInstance; //singleton instance

	//renderer를 위한 임시 배열입니다. 
	MapObject				m_Map[MAX_WIDTH][MAX_HEIGHT];

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

