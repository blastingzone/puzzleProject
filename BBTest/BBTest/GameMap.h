#pragma once

#include "MapObj.h"
#include "Dot.h"
#include "Line.h"
#include "Tile.h"

#define MAX_WIDTH	21
#define MAX_HEIGHT	21

enum LineDirection{
	HORIZONTAL,
	VERTICAL
};

typedef struct Edge{
	int				row;
	int				column;
	LineDirection	direction;
};

class CGameMap
{
public:
	CGameMap(void);
	~CGameMap(void);

public:
	static CGameMap*	GetInstance();
	static void			Release();

protected:
	int m_Width;
	int m_Height;

	static CGameMap*	m_pInstance; //singleton instance
	
	//renderer�� ���� �ӽ� �迭�Դϴ�. ������!
	CMapObj*			m_Map[MAX_WIDTH][MAX_HEIGHT];

	void createMap();
	bool isPossible(int row, int column);

public:
	void init();
	bool drawLine(Edge tempEdge); //success : return true
};

