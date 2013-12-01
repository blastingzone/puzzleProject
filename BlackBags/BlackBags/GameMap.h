#pragma once
#include "d2d1.h"
#include "SceneObject.h"
#include <array>

/*	게임 맵의 타일 좌표를 표현하기 위한 구조체 */
struct IndexedPosition
{
	IndexedPosition() : m_PosI(0), m_PosJ(0) {}

	IndexedPosition(const IndexedPosition& inputIndexedPosition)
	{
		m_PosI = inputIndexedPosition.m_PosI;
		m_PosJ = inputIndexedPosition.m_PosJ;
	}

	/*	복사 대입 연산자 오버라이딩
		queue같은 곳에 push해도 복사를 잘 해줍니당 */
	IndexedPosition& operator=(const IndexedPosition& inputIndexedPosition) 
	{
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

struct MapObject
{
	MapObject () : m_Type(MO_SENTINEL), m_Owner(MO_NOBODY), m_Item(MO_NOTHING), m_Flag(false) {}

	MO_TYPE		m_Type;
	MO_OWNER	m_Owner;
	MO_ITEM		m_Item;
	
	bool m_Flag;
};

class CGameMap : public CSceneObject
{
public:
	CGameMap(MapSize mapsize);
	~CGameMap(void);

	bool Init();
	void Render();

	void SetMapSize(MapSize mapsize);

	/*	입력된 좌표의 오브젝트를 연결 된 상태로 변경 */
	void DrawLine(const IndexedPosition& indexedPosition);

	void ShowVirtualLine(const IndexedPosition& indexedPosition);


	/*	입력된 좌표의 오브젝트를 연결되지 않은 상태로 변경
		(게임을 생성하면서 랜덤으로 울타리 배칠 할 때 사용됨) */
	void DeleteLine(const IndexedPosition& indexedPosition);

	D2D1_SIZE_F GetStartPosition() {return m_StartPosition;}

	MO_TYPE		GetMapType(IndexedPosition indexedPosition);
	MO_TYPE		GetMapType(const int& i, const int& j);

	void		SetMapOwner(IndexedPosition indexedPosition, MO_OWNER owner);
	MO_OWNER	GetMapOwner(IndexedPosition indexedPosition);
	MO_OWNER	GetMapOwner( const int& i , const int& j );

	void		SetItem(IndexedPosition indexedPosition, MO_ITEM item);
	MO_ITEM		GetItem(IndexedPosition indexedPosition) {return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Item;}

	/*	타일 탐색 시 탐색 여부를 확인하는 flag 지정 및 확인 */
	void		SetMapFlag(IndexedPosition indexedPosition,bool flag);
	bool		GetMapFlag(IndexedPosition indexedPosition) {return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Flag;}

	/*	맵 오프젝트 크기 반환하는 함수들 */
	float		GetTileSize(){ return m_TileSize; };
	float		GetLineWeight() { return m_LineWeight; }
	
	void SubtractVoidCount() { --m_VoidTileCount; }
	int	GetVoidTileCount() { return m_VoidTileCount; }

	void ResizeClient();

	/*	화면에 표시될 SceneObject 크기 지정 */
	void SetObjectSize();

	// 게임 결과를 GameData 클래스에 기록하는 메소드
	void WriteResult();

private: 
	bool CreateResource();

	/*	타일의 가로, 세로 숫자만 인자로 받아서 필요한 영역의 
		MapObject 속성을 변경 시켜서 맵을 생성 */
	void CreateMap();

	/*	현재 크기에 맞춰서 m_StartPosition의 좌표를 계산 */
	void CalcStartPosition();

	/*	주인이 없는 타일의 수를 저장하는 변수
		종료 조건을 쉽게 확인하기 위해서 생성 */
	int	m_VoidTileCount;

	/*	게임 데이터를 저장하기 위한 배열
		생성은 최대 크기로 생성하고, 
		게임 설정에 따라서 필요한 부분만 다른 MapObject로 속성 변경해서 사용 */
	std::array<std::array<MapObject, MAX_MAP_HEIGHT>, MAX_MAP_WIDTH> m_Map;

	/*	실제 게임 설정에 따른 필요한 맵 크기
		울타리나 점은 고려하지 않은 순수한 타일 수만 의미 */
	MapSize	m_MapSize;

	ID2D1HwndRenderTarget* m_pRenderTarget;

	/*	게임 맵을 그리기위한 화면 기준 점
		게임 맵의 가장 왼쪽 상단 위치 */
	D2D1_SIZE_F	m_StartPosition;

	float m_TileSize;
	float m_LineWeight;
	float m_DotRadius;
	float m_ItemRadius;

	ID2D1SolidColorBrush*	m_pDotBrush;
	ID2D1SolidColorBrush*	m_pUnconnectedLineBrush;
	ID2D1SolidColorBrush*	m_pConnectedLineBrush;
	ID2D1SolidColorBrush*	m_pPossibleLineBrush;
	ID2D1SolidColorBrush*	m_pTileBrush;
	ID2D1SolidColorBrush*	m_pVoidTileBrush;

	ID2D1SolidColorBrush*	m_pGoldBrush;
	ID2D1SolidColorBrush*	m_pTrashBrush;

	ID2D1SolidColorBrush*	m_pTileP1;
	ID2D1SolidColorBrush*	m_pTileP2;
	ID2D1SolidColorBrush*	m_pTileP3;
	ID2D1SolidColorBrush*	m_pTileP4;

	D2D1_ELLIPSE			m_DotEllipse;
};

