#pragma once
#include "d2d1.h"
#include "SceneObject.h"

#define MAX_WIDTH	21 //SM9: 이름이 모호하다. 윈도우 사이즈를 표현할 때 이걸 많이 쓴다. 여기에서는 다른 의미니까.. 네이밍 바꾸고 config.h로 빼도록
#define MAX_HEIGHT	21

struct IndexedPosition
{
	IndexedPosition() : m_PosI(0), m_PosJ(0) {}

	IndexedPosition(const IndexedPosition& inputIndexedPosition)
	{
		m_PosI = inputIndexedPosition.m_PosI;
		m_PosJ = inputIndexedPosition.m_PosJ;
	}

	//SM9: 이 객체는 보니까 여기저기 대입도 많이 하는 것 같은데.. 그럴 경우 아래처럼 복사 대입 연산자가 불린다.
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

//SM9: 아래 enum 들이  GameMap안에서만 쓰이는게 아니기 때문에 따로 enum만 모아놓은 헤더로 빼는것도 방법.
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

	void SetMapOwner(IndexedPosition indexedPosition, MO_OWNER owner); //SM9: 같은 함수를 이렇게 2개씩 만들어 놓은 이유는 무엇인가? 
	void SetMapOwner(const int& iPos, const int& jPos, MO_OWNER owner);

	void SetItem(IndexedPosition indexedPosition, MO_ITEM item); //SM9: 꼭 필요한 경우 아니라면 이렇게 오버로딩은 안쓰는게 좋다. 이렇게 간단한 경우는 호출하는 부분에서 통일
	void SetItem(const int& iPos, const int& jPos, MO_ITEM item);

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
	MapObject				m_Map[MAX_WIDTH][MAX_HEIGHT];

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

