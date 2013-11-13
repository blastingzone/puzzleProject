#pragma once
#include "d2d1.h"
#include "SceneObject.h"

struct IndexedPosition
{
	IndexedPosition() : m_PosI(0), m_PosJ(0) {}

	IndexedPosition(const IndexedPosition& inputIndexedPosition)
	{
		m_PosI = inputIndexedPosition.m_PosI;
		m_PosJ = inputIndexedPosition.m_PosJ;
	}

	// 복사 대입 연산자 오버라이딩입니다
	IndexedPosition& operator=(const IndexedPosition& inputIndexedPosition) 
	{
		//아래에 브레이크 포인트 찍고 언제 불리는지 확인해보삼.
		m_PosI = inputIndexedPosition.m_PosI;
		m_PosJ = inputIndexedPosition.m_PosJ;
		return *this ;
	}

	IndexedPosition(int PosI, int PosJ)
	{
		m_PosI = PosI;
		m_PosJ = PosJ;
	}

	int m_PosI;
	int m_PosJ;
};

struct MapSize
{
	int m_Width;
	int m_Height;
};

struct MapObject
{
	MapObject () : m_Type(MO_SENTINEL), m_Owner(MO_NOBODY), m_Item(MO_NOTHING), m_Flag(false) {}

	MO_TYPE		m_Type;
	MO_OWNER	m_Owner;
	MO_ITEM		m_Item;
	
	bool m_Flag;
};

/*
const float m_TileSize = 80.0f;
const float m_LineWeight = 10.0f;
const float m_DotRadius = 6.0f;
*/

class CGameMap : public CSceneObject
{
public:
	CGameMap(void);
	~CGameMap(void);

	void				Init();
	//static CGameMap*	GetInstance();
	//bool				ReleaseInstance();
	bool				Release();

	void				Render();

	void SetMapSize(MapSize mapsize);

	void DrawLine(const IndexedPosition& indexedPosition);

	void DeleteLine(const IndexedPosition& indexedPosition);

	D2D1_SIZE_F GetStartPosition();

	MO_TYPE	GetMapType(IndexedPosition indexedPosition);
	MO_TYPE GetMapType(const int& i, const int& j);

	MO_OWNER GetMapOwner(IndexedPosition indexedPosition);
	MO_OWNER GetMapOwner( const int& i , const int& j );

	void SetMapOwner(IndexedPosition indexedPosition, MO_OWNER owner);

	void SetItem(IndexedPosition indexedPosition, MO_ITEM item);

	bool GetMapFlag(IndexedPosition indexedPosition){return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Flag;}
	void SetMapFlag(IndexedPosition indexedPosition,bool flag);

	void SubtractVoidCount() { --m_VoidTileCount; }
	int	GetVoidTileCount() { return m_VoidTileCount; }

	void ResizeClient();
	void SetObjectSize();

private: 
	void CreateMap();
	bool CreateResource();
	void CalcStartPosition();

	int			m_VoidTileCount;
	MapSize		m_MapSize;
	D2D1_SIZE_F	m_StartPosition;

	//static CGameMap*	m_pInstance; //singleton instance

	//renderer를 위한 임시 배열입니다. 
	MapObject				m_Map[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

	ID2D1HwndRenderTarget*	m_pRenderTarget;

	float m_TileSize;
	float m_LineWeight;
	float m_DotRadius;

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

