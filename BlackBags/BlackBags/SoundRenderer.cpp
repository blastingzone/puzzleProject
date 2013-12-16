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
	// 조심해!!
	// assert 다 고쳐야해!!

	FMOD_RESULT       fr = FMOD::System_Create( &m_System );

	if (fr == FMOD_OK)
		fr = m_System->init( 32, FMOD_INIT_NORMAL, 0 );

	if (fr == FMOD_OK)
		fr = m_System->createChannelGroup( "MasterChannelGroup", &m_ChannelGroup );

	if (fr == FMOD_OK)
		fr = CreateSound();

	if (fr !=FMOD_OK)
		return false;


	// 없어도 되는 함수;;
	// AllocateChannel();

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
		fr = m_System->createSound(_SE_DESC, FMOD_HARDWARE, 0, &m_SE);

	return fr;
}

// 없어도 되는 함수
// 예제를 보니 얘네들을 따로 초기화해서 쓰는게 아니라 PlaySound 이후에 선언해야 한다
void CSoundRenderer::AllocateChannel()
{
	//FMOD_RESULT fr;

	//fr = m_SEChannel->setChannelGroup(m_ChannelGroup);

	//assert(fr == FMOD_OK);

	//fr = m_BGMChannel->setChannelGroup(m_ChannelGroup);

	//assert(fr == FMOD_OK);
}

void CSoundRenderer::PlayBGM(SOUND_BGMList BGMName)
{
	FMOD_RESULT fr;

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

}

void CSoundRenderer::StopBGM()
{
	FMOD_RESULT fr;

	fr = m_BGMChannel->stop();

	assert(fr == FMOD_OK);
}

void CSoundRenderer::PlaySE()
{
	FMOD_RESULT fr;

	fr = m_System->playSound(m_SE, m_ChannelGroup, false, &m_SEChannel);

	assert(fr == FMOD_OK);
}