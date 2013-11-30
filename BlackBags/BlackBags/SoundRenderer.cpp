#include "stdafx.h"
#include "SoundRenderer.h"

CSoundRenderer* CSoundRenderer::m_pInstance = nullptr;

CSoundRenderer::CSoundRenderer(void) :
	m_System(nullptr),
	m_ChannelGroup(nullptr),
	m_BGMChannel(nullptr),
	m_SEChannel(nullptr),
	m_SE(nullptr),
	m_BGM(nullptr)
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
	FMOD_RESULT       fr = m_System->createSound("Resource/Sound/BGM.mp3", FMOD_HARDWARE, 0, &m_BGM);

	if (fr == FMOD_OK)
	{
		fr = m_System->createSound("Resource/Sound/SE1.mp3", FMOD_HARDWARE, 0, &m_SE);
	}

	return fr;
}

void CSoundRenderer::AllocateChannel()
{
	m_SEChannel->setChannelGroup(m_ChannelGroup);
	m_BGMChannel->setChannelGroup(m_ChannelGroup);
}

// ������!!
// ��� ���� fr�� �޾Ƴ��⸸ �ϰ� void�� �������ִµ� fr�� Ȱ���ϵ��� �ٲ�� �մϴ�
void CSoundRenderer::PlayBGM()
{
	m_System->playSound(m_BGM, m_ChannelGroup, false, &m_BGMChannel);
}

void CSoundRenderer::PlaySE()
{
	m_System->playSound(m_SE, m_ChannelGroup, false, &m_SEChannel);
}