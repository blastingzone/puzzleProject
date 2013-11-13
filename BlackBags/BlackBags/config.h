/* ���� ȭ�� �⺻ ũ���Դϴ� */
const int WINDOW_WIDTH = 1250;
const int WINDOW_HEIGHT = 700;


/* �ִ� �÷��� ���� �ο��Դϴ� */
const int MAX_PLAYER_NUM= 4;


/* ���� �� ������Ʈ���� �⺻ ũ���Դϴ� */
const float DEFAULT_TILE_SIZE = 80.0f;
const float DEFAULT_LINE_WEIGHT = 10.0f;
const float DEFAULT_DOT_RADIUS = 6.0f;


/* ���� �� ũ���Դϴ� */
const int MAX_MAP_WIDTH = 21;
const int MAX_MAP_HEIGHT = 21;


/* ���� �ʱ�ȭ �� �� �� Ž���ϴ� �迭 ���� (�Լ� ���� ���� �� ���� ����) */
const int CHECKLIST_LENGTH = 100;


/* enum Ÿ�Ե��Դϴ�. */
enum MO_TYPE	//���� �� �⺻ �ڷ���
{
	MO_SENTINEL,
	MO_DOT = 10,
	MO_LINE_UNCONNECTED = 20,
	MO_LINE_CONNECTED,
	MO_LINE_HIDDEN,
	MO_TILE = 30,
};

enum MO_OWNER	//���� �� �� Ÿ�� ������
{
	MO_NOBODY = -1,
	MO_PLAYER1,
	MO_PLAYER2,
	MO_PLAYER3,
	MO_PLAYER4
};

enum MO_ITEM	//Ÿ�Ͽ� ������ ������
{
	MO_NOTHING,
	MO_GOLD,
	MO_TRASH
};
