#pragma once

#define MAX_CHAT_LEN	1024

#define MAX_NAME_LEN	30
#define MAX_COMMENT_LEN	40

enum PacketTypes
{
	PKT_NONE				= 0,

	PKT_CS_LOGIN			= 1,
	PKT_SC_LOGIN			= 2,

	PKT_CS_CHARACTER_SELECT = 3,
	PKT_SC_CHARACTER_SELECT = 4,

	PKT_CS_MAP_SELECT		= 5,
	PKT_SC_MAP_SELECT		= 6,

	PKT_CS_GAME_START		= 7,
	PKT_SC_GAME_START		= 8,

	PKT_CS_IDX				= 9,
	PKT_SC_IDX				= 10,

	PKT_CS_TURN_READY		= 11,
	PKT_SC_TURN_START		= 12,
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
	int		mConnectionNum;
	int		mCharacterId[MAX_PLAYER_NUM] ;
} ;

struct MapRequest : public PacketHeader
{
	MapRequest()
	{
		mSize = sizeof(MapRequest) ;
		mType = PKT_CS_MAP_SELECT ;
		mPlayerId = -1 ;
		mMapIdx = -1;
	}

	//방장만 선택할 수 있게 바꿀 것.
	int		mPlayerId ;
	int		mMapIdx;
} ;

struct MapResult : public PacketHeader
{
	MapResult()
	{
		mSize = sizeof(MapResult) ;
		mType = PKT_SC_MAP_SELECT ;
		mMapIdx = -1;
	}
	int		mMapIdx;
} ;

struct GameStartRequest : public PacketHeader
{
	GameStartRequest()
	{
		mSize = sizeof(MapResult) ;
		mType = PKT_CS_GAME_START ;
		mStart = false;
	}
	bool mStart;
} ;

struct GameStartResult : public PacketHeader
{
	GameStartResult()
	{
		mSize = sizeof(MapResult) ;
		mType = PKT_SC_GAME_START ;
		mStart = false;
		randomSeed = 0;
	}
	int mStart;
	unsigned int randomSeed;
} ;

// 현재 턴인 클라이언트가 어떤 선을 골랐는지 서버에 통보
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
};

// 현재 턴인 클라이언트가 어떤 선을 골랐는지 각 클라이언트에 방송
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
};

// 용도 : 클라이언트가 다음 턴으로 진행할 준비가 되었다 (애니메이션이 끝남)
struct TurnReadyRequest : public PacketHeader
{
	TurnReadyRequest()
	{
		mSize = sizeof(TurnReadyRequest);
		mType = PKT_CS_TURN_READY;
		mClientId = -1;
	}
	
	int mClientId;
};

// 용도 : 다음 턴으로 바꾸고 타이머를 시작
struct TurnStartResult : public PacketHeader
{
	TurnStartResult()
	{
		mSize = sizeof(TurnStartResult);
		mType = PKT_SC_TURN_START;
		mNextTurnId = -1;
	}

	int mNextTurnId;
};

#pragma pack(pop)