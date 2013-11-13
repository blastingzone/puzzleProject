/* 게임 화면 기본 크기입니다 */
const int WINDOW_WIDTH = 1250;
const int WINDOW_HEIGHT = 700;


/* 최대 플레이 가능 인원입니다 */
const int MAX_PLAYER_NUM= 4;


/* 게임 맵 오브젝트들의 기본 크기입니다 */
const float DEFAULT_TILE_SIZE = 80.0f;
const float DEFAULT_LINE_WEIGHT = 10.0f;
const float DEFAULT_DOT_RADIUS = 6.0f;


/* 게임 맵 크기입니다 */
const int MAX_MAP_WIDTH = 21;
const int MAX_MAP_HEIGHT = 21;


/* 지도 초기화 할 때 맵 탐사하는 배열 길이 (함수 구조 변경 후 삭제 예정) */
const int CHECKLIST_LENGTH = 100;


/* enum 타입들입니다. */
enum MO_TYPE	//게임 맵 기본 자료형
{
	MO_SENTINEL,
	MO_DOT = 10,
	MO_LINE_UNCONNECTED = 20,
	MO_LINE_CONNECTED,
	MO_LINE_HIDDEN,
	MO_TILE = 30,
};

enum MO_OWNER	//게임 맵 각 타일 소유주
{
	MO_NOBODY = -1,
	MO_PLAYER1,
	MO_PLAYER2,
	MO_PLAYER3,
	MO_PLAYER4
};

enum MO_ITEM	//타일에 지정된 아이템
{
	MO_NOTHING,
	MO_GOLD,
	MO_TRASH
};
