#pragma once
#include "Config.h"

#define MAX_CHAT_LEN	1024

#define MAX_NAME_LEN	30
#define MAX_COMMENT_LEN	40

enum PacketTypes
{
	PKT_NONE	= 0,

	PKT_CS_LOGIN	= 1,
	PKT_SC_LOGIN	= 2,

	PKT_CS_CHARACTER_SELECT = 3,
	PKT_SC_CHARACTER_SELECT = 4,

	PKT_CS_IDX		= 5,
	PKT_SC_IDX		= 6,
} ;

#pragma pack(push, 1)

struct PacketHeader
{
	PacketHeader() : mSize(0), mType(PKT_NONE) 	{}
	short mSize ;
	short mType ;
} ;



struct LoginRequest : public PacketHeader
{
	LoginRequest()
	{
		mSize = sizeof(LoginRequest) ;
		mType = PKT_CS_LOGIN ;
		mPlayerId = -1 ;
	}

	int	mPlayerId ;
} ;

struct LoginResult : public PacketHeader
{
	LoginResult()
	{
		mSize = sizeof(LoginResult) ;
		mType = PKT_SC_LOGIN ;
		mPlayerId = -1 ;
		memset(mName, 0, MAX_NAME_LEN) ;
	}

	int		mPlayerId ;
	char	mName[MAX_NAME_LEN] ;

} ;

struct CharacterRequest : public PacketHeader
{
	CharacterRequest()
	{
		mSize = sizeof(CharacterRequest) ;
		mType = PKT_CS_CHARACTER_SELECT ;
		mPlayerId = -1 ;
		mCharacterId = -1;
	}

	int		mPlayerId ;
	int		mCharacterId ;
} ;

struct CharacterResult : public PacketHeader
{
	CharacterResult()
	{
		mSize = sizeof(CharacterResult) ;
		mType = PKT_SC_CHARACTER_SELECT ;
		// 인덱스가 플레이어 - 밸류가 캐릭터
		memset( mCharacterId, -1, sizeof(mCharacterId) );
	}
	int		mCharacterId[MAX_CLIENT_NUM] ;
} ;


struct EventPositionRequest : public PacketHeader
{
	EventPositionRequest()
	{
		mSize = sizeof(EventPositionRequest);
		mType = PKT_CS_IDX;
		mPlayerId = -1 ;

		m_Xpos = 0;
		m_Ypos = 0;
	}

	int	mPlayerId ;
	int m_Xpos;
	int m_Ypos;
	char mName[MAX_NAME_LEN];
};

struct EventPositionResult : public PacketHeader
{
	EventPositionResult()
	{
		mSize = sizeof(EventPositionResult);
		mType = PKT_SC_IDX;
		mPlayerId = -1 ;

		m_Xpos = 0;
		m_Ypos = 0;
	}

	int	mPlayerId ;
	int m_Xpos;
	int m_Ypos;
	char mName[MAX_NAME_LEN];

};
#pragma pack(pop)