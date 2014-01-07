#pragma once


/// DB 연결 스트링 
#define DB_CONN_STR "..\\Database\\user_example.db3"
#define LISTEN_PORT	22222


/// 1분마다 플레이어 상태 저장
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