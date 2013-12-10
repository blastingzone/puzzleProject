/*	게임 화면 기본 크기입니다 */
const int WINDOW_WIDTH = 1250;
const int WINDOW_HEIGHT = 700;


/*	최대 플레이 가능 인원입니다 */
const int MAX_PLAYER_NUM= 4;


/*	메인 메뉴 오브젝트들의 기본 크기입니다 */
const float SC_M_DEFAULT_MENU_BUTTON_WIDTH = 290.0f;
const float SC_M_DEFAULT_MENU_BUTTON_HEIGHT = 40.0f;
const float SC_M_DEFAULT_TEXT_MARGIN = 15.0F;
const float SC_M_DEFAULT_TEXT_SIZE = 20.0F;

/*	세팅 메뉴 오브젝트들의 기본 크기입니다	*/
const float SC_S_DEFAULT_PLAYER_BUTTON_WIDTH = 290.0f;
const float SC_S_DEFAULT_PLAYER_BUTTON_HEIGHT = 40.0f;

const float SC_S_DEFAULT_PLAYER_TEXT_MARGIN = 15.0F;
const float SC_S_DEFAULT_PLAYER_TEXT_SIZE = 15.0F;

const float SC_S_DEFAULT_MAP_BUTTON_WIDTH = 290.0f;
const float SC_S_DEFAULT_MAP_BUTTON_HEIGHT = 40.0f;

const float SC_S_DEFAULT_MAP_TEXT_MARGIN = 15.0F;
const float SC_S_DEFAULT_MAP_TEXT_SIZE = 20.0F;

const float SC_S_DEFAULT_NEXT_BUTTON_WIDTH = 290.0f;
const float SC_S_DEFAULT_NEXT_BUTTON_HEIGHT = 40.0f;

const float SC_S_DEFAULT_NEXT_TEXT_MARGIN = 15.0f;
const float SC_S_DEFAULT_NEXT_TEXT_SIZE = 15.0f;

const float SC_S_DEFAULT_SUBTITLE_LAYER_WIDTH = 300.0f;
const float SC_S_DEFAULT_SUBTITLE_LAYER_HEIGHT = 40.f;

const float SC_S_DEFAULT_SUBTITLE_TEXT_SIZE = 40.0f;
const float SC_S_DEFAULT_SUBTITLE_TEXT_MARGIN = 15.0f;

const float SC_S_DEFAULT_MAINTITLE_LAYER_WIDTH = 300.0f;
const float SC_S_DEFAULT_MAINTITLE_LAYER_HEIGHT = 40.f;

const float SC_S_DEFAULT_MAINTITLE_TEXT_SIZE = 60.0f;
const float SC_S_DEFAULT_MAINTITLE_TEXT_MARGIN = 20.0f;

/*	settingScene 초상화 크기	*/
const float SC_S_DEFAULT_PORTRAIT_WIDTH = 200.f;
const float SC_S_DEFAULT_PORTRAIT_HEIGHT = 200.f;

/*	판정을 위해서 얘네들의 y 포지션을 받아둡니다 */
const int	SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE = 5;
const int   SC_S_DEFAULT_NEXT_BUTTON_Y_POSITION_SCALE = 10;
const int   SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE = 8;

/*	어떤 플레이어가 선택되었는지 알기 위한 MASK 값들	*/
const int	MASK_PLAYER_1 = 1;
const int	MASK_PLAYER_2 = 2;
const int	MASK_PLAYER_3 = 4;
const int	MASK_PLAYER_4 = 8;

/*	게임 맵 오브젝트들의 기본 크기입니다 */
const float DEFAULT_TILE_SIZE = 80.0f;
const float DEFAULT_LINE_WEIGHT = 10.0f;
const float DEFAULT_DOT_RADIUS = 6.0f;
const float DEFAULT_ITEM_RADIUS = 4.0f;
const float DEFAULT_CHARACTER_SIZE = 150.0f;
const float DEFAULT_CHARACTER_BOX_WIDTH = 150.0f;
const float DEFAULT_CHARACTER_BOX_HEIGHT = 30.0f;
const float DEFAULT_CHARACTER_MARGIN_H =600.0f;
const float DEFAULT_CHARACTER_MARGIN_V = 300.0f;

const float SC_P_TIMER_POSITION_HEIGHT = 10.0f;
const float SC_P_TIMER_WIDTH = 500.0f;
const float SC_P_TIMER_HEIGHT = 5.0f;

const DWORD TIME_LIMIT = 20 * 1000;

const float SC_P_CLICK_BUFFER = 7.0f;

const DWORD SC_P_LINE_ANIMATION_TIME = 500;
const DWORD SC_P_TILE_ANIMATION_TIME = 500;

/*	게임 맵 크기입니다 */
const int MAX_MAP_WIDTH = 23;
const int MAX_MAP_HEIGHT = 23;


/*	지도 초기화 할 때 맵 탐사하는 배열 길이 (함수 구조 변경 후 삭제 예정) */
const int CHECKLIST_LENGTH = 100;


/*	결과 화면 오브젝트들의 기본 크기입니다 */
const float SC_RT_HORIZONTAL_MARGIN = 63.0f;
const float SC_RT_VERTICAL_MARGIN = 34.0f;
const float SC_RT_VOID_SPACE = 66.0f;

const float SC_RT_TITLE_WIDTH = 340.0f;
const float SC_RT_TITLE_HEIGHT = 72.0f;
const float SC_RT_TITLE_TEXT_SIZE = 72.0f;

const float SC_RT_PLAYER_NAME_TEXT_SIZE = 36.0f;
const float SC_RT_PLAYER_NAME_TEXT_WIDTH = 240.0f;
const float SC_RT_PLAYER_NAME_TEXT_HEIGHT = 48.0f;
const float SC_RT_PLAYER_SCORE_TEXT_SIZE = 36.0f;
const float SC_RT_PLAYER_SCORE_POSITION = 680.0f;
const float SC_RT_PLAYER_SCORE_WIDTH = 80.0f;
const float SC_RT_PLAYER_TILE_SIZE = 18.0f;
const float SC_RT_PLAYER_TILE_MARGIN = 2.5f;
const float SC_RT_PLAYER_BOX_HEIGHT = 92.0f;

const float SC_RT_WINNER_TEXT_SIZE = 48.0f;
const float SC_RT_WINNER_TEXT_WIDTH = 326.0f;

const float SC_RT_BUTTON_WIDTH = 210.0f;
const float SC_RT_BUTTON_HEIGHT = 60.0f;
const float SC_RT_BUTTON_TEXT_SIZE = 36.0f;
const float SC_RT_BUTTON_TEXT_MARGIN = 0.0f;

const int	SC_RT_SCORE_TILE = 2;
const int	SC_RT_SCORE_GOLD = 5;
const int	SC_RT_SCORE_TRASH = -10;

/*	크레딧 신에서 불러올 동영상들입니다	*/
#define _CREDIT_MOVIE "Resource/Video/credit.avi"
#define _OPENING_MOVIE "Resource/Video/opening.avi"

/*	폰트들입니다	*/
#define _DEBUG_FONT L"Consolas"
#define _MENU_FONT L"Segoe UI"

/*	플레이어 색상입니다	*/
#define _COLOR_PLAYER_1_ 78.0f/255, 213.0f/255, 199.0f/255
#define _COLOR_PLAYER_2_ 247.0f/255, 166.0f/255, 123.0f/255
#define _COLOR_PLAYER_3_ 212.0f/255, 72.0f/255, 101.0f/255
#define _COLOR_PLAYER_4_ 184.0f/255, 218.0f/255, 141.0f/255

/*	enum 타입들입니다. */
enum SceneName	//Scene 이름들
{
	SC_OPENING,
	SC_MAIN,
	SC_SETTING,
	SC_PLAY,
	SC_RESULT,
	SC_CREDIT,
	SC_NOSCENE,
	SC_EXIT
};

enum SOUND_BGMList
{
	BGM_MAIN,
	BGM_SETTING,
	BGM_PLAY,
	BGM_RESULT
};

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

enum Direction	//게임 맵 탐색 시 활용되는 방향
{
	DI_UP,
	DI_RIGHT,
	DI_DOWN,
	DI_LEFT
};

struct MapSize //맵 크기값을 위한 구조체입니다
{
	int m_Width;
	int m_Height;
};