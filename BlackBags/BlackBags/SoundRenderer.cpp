#include "stdafx.h"
#include "SoundRenderer.h"

CSoundRenderer* CSoundRenderer::m_pInstance = nullptr;

CSoundRenderer::CSoundRenderer(void) :
	m_System(nullptr),
	m_ChannelGroup(nullptr),
	m_BGMChannel(nullptr),
	m_SEChannel(nullptr),
	m_SE(nullptr),
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
	{
		fr = m_System->init( 32, FMOD_INIT_NORMAL, 0 );
	}

	if (fr == FMOD_OK)
	{
		fr = m_System->createChannelGroup( "MasterChannelGroup", &m_ChannelGroup );
	}

	if (fr == FMOD_OK)
	{
		fr = CreateSound();
	}

	if (fr == FMOD_OK)
	{
		AllocateChannel();
		return true;
	}

	return false;
}

FMOD_RESULT CSoundRenderer::CreateSound()
{
	FMOD_RESULT fr = m_System->createSound("Resource/Sound/SE1.mp3", FMOD_HARDWARE, 0, &m_Main);

	//문진상 조심해!!
	if (fr == FMOD_OK)
	{
		fr = m_System->createSound("Resource/Sound/SE1.mp3", FMOD_HARDWARE, 0, &m_Setting);
	}

	if (fr == FMOD_OK)
	{
		fr = m_System->createSound("Resource/Sound/SE1.mp3", FMOD_HARDWARE, 0, &m_Play);
	}

	if (fr == FMOD_OK)
	{
		fr = m_System->createSound("Resource/Sound/SE1.mp3", FMOD_HARDWARE, 0, &m_Result);
	}

	if (fr == FMOD_OK)
	{
		fr = m_System->createSound("Resource/Sound/SE1.mp3", FMOD_HARDWARE, 0, &m_SE);
	}

	return fr;
}

void CSoundRenderer::AllocateChannel()
{
	//문진상 조심해 애러챜
	m_SEChannel->setChannelGroup(m_ChannelGroup);
	m_BGMChannel->setChannelGroup(m_ChannelGroup);
}

// 문진상 조심해!!
// 얘네 둘이 fr에 받아놓기만 하고 void로 리턴해주는데 fr값 활용하도록 바꿔야 합니다 FMOD_RESULT
void CSoundRenderer::PlayBGM(SOUND_BGMList BGMName)
{
	switch (BGMName)
	{
	case BGM_MAIN:
		m_System->playSound(m_Main, m_ChannelGroup, false, &m_BGMChannel);
		break;
	case BGM_SETTING:
		m_System->playSound(m_Setting, m_ChannelGroup, false, &m_BGMChannel);
		break;
	case BGM_PLAY:
		m_System->playSound(m_Play, m_ChannelGroup, false, &m_BGMChannel);
		break;
	case BGM_RESULT:
		m_System->playSound(m_Result, m_ChannelGroup, false, &m_BGMChannel);
		break;
	default:
		break;
	}
}

void CSoundRenderer::StopBGM()
{
	m_BGMChannel->stop();
}

void CSoundRenderer::PlaySE()
{
	m_System->playSound(m_SE, m_ChannelGroup, false, &m_SEChannel);
}