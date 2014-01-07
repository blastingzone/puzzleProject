#include "stdafx.h"
#include "SoundRenderer.h"

CSoundRenderer* CSoundRenderer::m_pInstance = nullptr;

CSoundRenderer::CSoundRenderer(void) :
	m_System(nullptr),
	m_ChannelGroup(nullptr),
	m_BGMChannel(nullptr),
	m_SEChannel(nullptr),
	m_PlayerChannel(nullptr),
	m_SE_DrawLine(nullptr),
	m_SE_FillTile(nullptr),
	m_SE_Select(nullptr),
	m_SE_Player1(nullptr),
	m_SE_Player2(nullptr),
	m_SE_Player3(nullptr),
	m_SE_Player4(nullptr),
	m_Main(nullptr),
	m_Setting(nullptr),
	m_Play(nullptr),
	m_Result(nullptr)
{
}


CSoundRenderer::~CSoundRenderer(void)
{
}

CSoundRenderer* CSoundRenderer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CSoundRenderer();
	}

	return m_pInstance;
}

void CSoundRenderer::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}

bool CSoundRenderer::Init()
{
	FMOD_RESULT       fr = FMOD::System_Create( &m_System );

	if (fr == FMOD_OK)
		fr = m_System->init( 32, FMOD_INIT_NORMAL, 0 );

	if (fr == FMOD_OK)
		fr = m_System->createChannelGroup( "MasterChannelGroup", &m_ChannelGroup );

	if (fr == FMOD_OK)
		fr = CreateSound();

	if (fr !=FMOD_OK)
		return false;

	return true;
}

FMOD_RESULT CSoundRenderer::CreateSound()
{
	FMOD_RESULT fr = m_System->createSound(_BGM_MAIN, FMOD_HARDWARE, 0, &m_Main);

	// assert 추가로 경로에 없는 파일이 있으면 해당 위치에서 프로그램 죽음
	if (fr == FMOD_OK)
		fr = m_System->createSound(_BGM_SETTING, FMOD_HARDWARE, 0, &m_Setting);

	if (fr == FMOD_OK)
		fr = m_System->createSound(_BGM_PLAY, FMOD_HARDWARE, 0, &m_Play);

	if (fr == FMOD_OK)
		fr = m_System->createSound(_BGM_RESULT, FMOD_HARDWARE, 0, &m_Result);

	if (fr == FMOD_OK)
		fr = m_System->createSound(_SE_DRAW_LINE, FMOD_HARDWARE, 0, &m_SE_DrawLine);

	if (fr == FMOD_OK)
		fr = m_System->createSound(_SE_SELECT, FMOD_HARDWARE, 0, &m_SE_Select);

	if ( fr == FMOD_OK)
		fr = m_System->createSound(_SE_LAUGH, FMOD_HARDWARE, 0, &m_SE_Laugh);

	if ( fr == FMOD_OK)
		fr = m_System->createSound(_SE_PLAYER1, FMOD_HARDWARE, 0, &m_SE_Player1);

	if ( fr == FMOD_OK)
		fr = m_System->createSound(_SE_PLAYER2, FMOD_HARDWARE, 0, &m_SE_Player2);

	if ( fr == FMOD_OK)
		fr = m_System->createSound(_SE_PLAYER3, FMOD_HARDWARE, 0, &m_SE_Player3);

	if ( fr == FMOD_OK)
		fr = m_System->createSound(_SE_PLAYER4, FMOD_HARDWARE, 0, &m_SE_Player4);

	if ( fr == FMOD_OK)
		fr = m_System->createSound(_SE_FILL_TILE, FMOD_HARDWARE, 0, &m_SE_FillTile);	
	
	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}

	return fr;
}

void CSoundRenderer::PlayBGM(SOUND_BGMList BGMName)
{
	FMOD_RESULT fr = FMOD_OK;

	switch (BGMName)
	{
	case BGM_MAIN:
		fr = m_System->playSound(m_Main, m_ChannelGroup, false, &m_BGMChannel);
		//assert(fr == FMOD_OK);
		break;
	case BGM_SETTING:
		fr = m_System->playSound(m_Setting, m_ChannelGroup, false, &m_BGMChannel);
		//assert(fr == FMOD_OK);
		break;
	case BGM_PLAY:
		fr = m_System->playSound(m_Play, m_ChannelGroup, false, &m_BGMChannel);
		//assert(fr == FMOD_OK);
		break;
	case BGM_RESULT:
		fr = m_System->playSound(m_Result, m_ChannelGroup, false, &m_BGMChannel);
		//assert(fr == FMOD_OK);
		break;
	default:
		break;
	}

	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}
}

void CSoundRenderer::StopBGM()
{
	FMOD_RESULT fr;

	fr = m_BGMChannel->stop();

	//assert(fr == FMOD_OK);
	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}
}

void CSoundRenderer::PlaySE_DrawLine()
{
	FMOD_RESULT fr;

	fr = m_System->playSound(m_SE_DrawLine, m_ChannelGroup, false, &m_SEChannel);

	assert(fr == FMOD_OK);
	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}
}

void CSoundRenderer::PlaySE_Select()
{
	FMOD_RESULT fr;

	fr = m_System->playSound(m_SE_Select, m_ChannelGroup, false, &m_SEChannel);

	assert(fr == FMOD_OK);
	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}
}

void CSoundRenderer::PlaySE_Laugh()
{
	FMOD_RESULT fr;

	fr = m_System->playSound(m_SE_Laugh, m_ChannelGroup, false, &m_SEChannel);

	assert(fr == FMOD_OK);
	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}
}



void CSoundRenderer::PlaySE_Player( int playerId )
{
	FMOD_RESULT fr;
	
	switch (playerId)
	{
	case 0:
		fr = m_System->playSound(m_SE_Player1, m_ChannelGroup, false, &m_PlayerChannel);
		break;
	case 1:
		fr = m_System->playSound(m_SE_Player2, m_ChannelGroup, false, &m_PlayerChannel);
		break;
	case 2:
		fr = m_System->playSound(m_SE_Player3, m_ChannelGroup, false, &m_PlayerChannel);
		break;
	case 3:
		fr = m_System->playSound(m_SE_Player4, m_ChannelGroup, false, &m_PlayerChannel);
		break;
	default:
		break;
	}

	assert(fr == FMOD_OK);
	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}
}


void CSoundRenderer::PlaySE_FillTile()
{
	FMOD_RESULT fr;

	fr = m_System->playSound(m_SE_FillTile, m_ChannelGroup, false, &m_SEChannel);

	assert(fr == FMOD_OK);
	if (!SUCCEEDED(fr) )
	{
		ErrorHandling();
	}
}

void CSoundRenderer::ErrorHandling()
{
	//해결 : MB_DEFAULT_DESKTOP_ONLY를 추가해준다!
	std::wstring errorText = L"Error Code : ";
	errorText.append( std::to_wstring( GetLastError() ) );
	MessageBox(NULL, errorText.c_str(), L"Error!", MB_ICONERROR|MB_DEFAULT_DESKTOP_ONLY);
	// 비정상 종료. 음악 안 나오는 경우는 에러만 보여주고 그냥 진행한다.
	// PostQuitMessage(-1);
}