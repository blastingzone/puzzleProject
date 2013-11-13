const int WINDOW_WIDTH = 1250;
const int WINDOW_HEIGHT = 700;

const int MAX_PLAYER_NUM= 4;

const float DEFAULT_TILE_SIZE = 80.0f;
const float DEFAULT_LINE_WEIGHT = 10.0f;
const float DEFAULT_DOT_RADIUS = 6.0f;

// 지도 크기입니다

const int MAX_MAP_WIDTH = 21;
const int MAX_MAP_HEIGHT = 21;

// 지도 초기화 할 때 맵 탐사하는 배열 길이

const int CHECKLIST_LENGTH = 100;

// enum 타입들입니다.

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
