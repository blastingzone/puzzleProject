#pragma once
#include "d2d1.h"
#include "SceneObject.h"
#include <array>
#include <dwrite.h>

/*	���� ���� Ÿ�� ��ǥ�� ǥ���ϱ� ���� ����ü */
struct IndexedPosition
{
	IndexedPosition() : m_PosI(0), m_PosJ(0) {}

	IndexedPosition(const IndexedPosition& inputIndexedPosition)
	{
		m_PosI = inputIndexedPosition.m_PosI;
		m_PosJ = inputIndexedPosition.m_PosJ;
	}

	/*	���� ���� ������ �������̵�
		queue���� ���� push�ص� ���縦 �� ���ݴϴ� */
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
	MapObject () :
		m_Type(MO_SENTINEL),
		m_Owner(MO_NOBODY),
		m_Item(MO_NOTHING),
		m_Flag(false),
		m_AnimationFlag(false),
		m_StartTime(0),
		m_AnimationTurn(0),
		m_Direction(DI_UP)
		{}

	MO_TYPE		m_Type;
	MO_OWNER	m_Owner;
	MO_ITEM		m_Item;
	
	bool m_Flag;
	bool m_MouseOverFlag;	

	bool		m_AnimationFlag;
	DWORD		m_StartTime;
	int			m_AnimationTurn;
	Direction	m_Direction;
};

class CNetworkGameMap : public CSceneObject
{
public:
	CNetworkGameMap(MapSize mapsize);
	virtual ~CNetworkGameMap(void);

	bool Init();
	void Render();

	void SetMapSize(MapSize mapsize);
	void DrawPlayerUI(int playerNumber);
	void GetPlayerUIPosition();

	/*	�Էµ� ��ǥ�� ������Ʈ�� ���� �� ���·� ���� */
	void DrawLine(const IndexedPosition& indexedPosition);
	void InitVirtualLineState();
	void ShowVirtualLine(const IndexedPosition& indexedPosition);

	/*	�Էµ� ��ǥ�� ������Ʈ�� ������� ���� ���·� ����
		(������ �����ϸ鼭 �������� ��Ÿ�� ��ĥ �� �� ����) */
	void DeleteLine(const IndexedPosition& indexedPosition);

	D2D1_SIZE_F GetStartPosition()							{ return m_StartPosition; }

	MO_TYPE		GetMapType(IndexedPosition indexedPosition);
	MO_TYPE		GetMapType(const int& i, const int& j);

	void		SetMapOwner(IndexedPosition indexedPosition, MO_OWNER owner);
	MO_OWNER	GetMapOwner(IndexedPosition indexedPosition);
	MO_OWNER	GetMapOwner( const int& i , const int& j );

	void		SetItem(IndexedPosition indexedPosition, MO_ITEM item);
	MO_ITEM		GetItem(IndexedPosition indexedPosition)	{ return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Item; }

	/*	Ÿ�� Ž�� �� Ž�� ���θ� Ȯ���ϴ� flag ���� �� Ȯ�� */
	void		SetMapFlag(IndexedPosition indexedPosition,bool flag);
	bool		GetMapFlag(IndexedPosition indexedPosition) { return m_Map[indexedPosition.m_PosI][indexedPosition.m_PosJ].m_Flag; }

	/*	�� ������Ʈ ũ�� ��ȯ�ϴ� �Լ��� */
	float		GetTileSize()								{ return m_TileSize; };
	float		GetLineWeight()								{ return m_LineWeight; }

	/*	�ִϸ��̼� ���¸� ����, ��ȯ�ϴ� �Լ��� */
	bool		GetLineAnimationFlag()						{ return m_LineAnimationFlag; }
	int			GetTileAnimationTurnNumber()				{ return m_TileAnimationTurnNumber; }
	void		SetTileAnimationTurnNumber(int turnNumber)	{ m_TileAnimationTurnNumber = turnNumber; }
	void		SetAnimationState(IndexedPosition indexedPosition, int turn, Direction direction);
	void		InitAnimationState(IndexedPosition indexedPosition);
	void		SetTileAnimationTurn(int turn)				{ m_TileAnimationTurn = turn; }
	int			GetTileAnimationTurn(IndexedPosition indexedPosition);
	
	void SubtractVoidCount()								{ --m_VoidTileCount; }
	int	GetVoidTileCount()									{ return m_VoidTileCount; }

	void ResizeClient();

	/*	ȭ�鿡 ǥ�õ� SceneObject ũ�� ���� */
	void SetObjectSize();

	// ���� ����� GameData Ŭ������ ����ϴ� �޼ҵ�
	void WriteResult();

	/*	player turn table ���� �Լ� */
	void SetPlayerTurnTable(int idx, CPlayer* playerPtr);

	/* ���� �ʿ��� ���� ���� ��� ���� �Լ� */
	void SetCurrentTurn(int turn)							{ m_CurrentTurn = turn; }

private: 
	bool CreateResource();

	/*	Ÿ���� ����, ���� ���ڸ� ���ڷ� �޾Ƽ� �ʿ��� ������ 
		MapObject �Ӽ��� ���� ���Ѽ� ���� ���� */
	void CreateMap();

	/*	���� ũ�⿡ ���缭 m_StartPosition�� ��ǥ�� ��� */
	void CalcStartPosition();

	/*	������ ���� Ÿ���� ���� �����ϴ� ����
		���� ������ ���� Ȯ���ϱ� ���ؼ� ���� */
	int	m_VoidTileCount;

	/*	���� �����͸� �����ϱ� ���� �迭
		������ �ִ� ũ��� �����ϰ�, 
		���� ������ ���� �ʿ��� �κи� �ٸ� MapObject�� �Ӽ� �����ؼ� ��� */
	//std::array<std::array<MapObject, MAX_MAP_HEIGHT>, MAX_MAP_WIDTH> m_Map;
	
	template <typename IType, int ROW, int COL>
	struct array2d_
	{
		typedef std::array< std::array<IType, COL>, ROW> type ;
	} ;

	typedef array2d_<MapObject, MAX_MAP_WIDTH, MAX_MAP_HEIGHT>::type MapObjMap ;
	MapObjMap m_Map;

	// ������!! �̷������� �׳� 2���� �迭 ���·� ���� ���ϰ���? 
	//@{
	/* ���� �� ���ÿ�
	template <typename IType, int ROW, int COL>
	struct array2d_
	{
		typedef std::array< std::array<IType, COL>, ROW> type ;
	} ;

	typedef array2d_<MapObject, MAX_MAP_WIDTH, MAX_MAP_HEIGHT>::type MapObjMap ;
	
	*/
	//@}


	/*	���� ���� ������ ���� �ʿ��� �� ũ��
		��Ÿ���� ���� ������� ���� ������ Ÿ�� ���� �ǹ� */
	MapSize	m_MapSize;

	ID2D1HwndRenderTarget* m_pRenderTarget;

	/*	���� ���� �׸������� ȭ�� ���� ��
		���� ���� ���� ���� ��� ��ġ */
	D2D1_SIZE_F	m_StartPosition;
	D2D1_SIZE_F  m_CenterPosition;

	float m_TileSize;
	float m_LineWeight;
	float m_DotRadius;
	float m_ItemRadius;
	float m_ProfileSize;
	float m_ProfileBoxHeight;
	float m_ProfileBoxWidth;
	float m_ProfileHorizontalMargin;
	float m_ProfileVerticalMargin;

	float m_TimerPositionHeight;
	float m_TimerWidth;
	float m_TimerHeight;

	std::array<D2D1_RECT_F, MAX_PLAYER_NUM> m_ProfilePosition;
	std::array<D2D1_RECT_F, MAX_PLAYER_NUM> m_ProfileBoxPosition;

	ID2D1SolidColorBrush*	m_pDotBrush;
	ID2D1SolidColorBrush*	m_pUnconnectedLineBrush;
	ID2D1SolidColorBrush*	m_pConnectedLineBrush;
	ID2D1SolidColorBrush*	m_pPossibleLineBrush;
	ID2D1SolidColorBrush*	m_pTileBrush;
	ID2D1SolidColorBrush*	m_pVoidTileBrush;

	ID2D1SolidColorBrush*	m_pGoldBrush;
	ID2D1SolidColorBrush*	m_pTrashBrush;

	ID2D1SolidColorBrush*	m_pTimer;

	D2D1_ELLIPSE			m_DotEllipse;

	bool	m_LineAnimationFlag;
	int		m_TileAnimationTurnNumber;
	int		m_TileAnimationTurn;

	ID2D1Bitmap* m_backImg;

	ID2D1Bitmap* m_gold;
	ID2D1Bitmap* m_trash;

	IDWriteFactory*			m_DWriteFactory;
	IDWriteTextFormat*		m_TextFormat;

	bool m_isMouseOn;

	//playScene�� �ߺ��Ǵ� ������
	std::array<CPlayer*, MAX_PLAYER_NUM> m_PlayerTurnTable;

	int m_CurrentTurn;
};

