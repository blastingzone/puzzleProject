#pragma once


/// DB ���� ��Ʈ�� 
#define DB_CONN_STR "..\\Database\\user_example.db3"
#define LISTEN_PORT	22222


/// 1�и��� �÷��̾� ���� ����
#define PLAYER_DB_UPDATE_CYCLE	60

#define MAX_CLIENT_NUM 4

#define NOT_SELECTED -1
#define NOT_LOGIN_CLIENT -1

enum GameScene
{
	SC_SETTING,
	SC_PLAY,
	SC_NOSCENE,
	SC_RESULT
};