#pragma once
#include <map>

#include "Library\FMOD\inc\fmod.hpp"
#include "Library\FMOD\inc\fmod_errors.h"

class CSoundRenderer
{
public:
	CSoundRenderer(void);
	~CSoundRenderer(void);

	bool Init();

	static CSoundRenderer*		CSoundRenderer::GetInstance();
	static void					ReleaseInstance();

	FMOD_RESULT CreateSound();
	void AllocateChannel();

	void PlayBGM();
	void StopBGM();
	void PlaySE();

	FMOD::System* GetSystem() { return m_System; }

private:
	static CSoundRenderer*	m_pInstance;

	FMOD::System*			m_System;
	FMOD::ChannelGroup*		m_ChannelGroup;

	//std::map<std::string, FMOD::Sound*> m_SoundTable;

	FMOD::Channel*			m_BGMChannel;
	FMOD::Channel*			m_SEChannel;

	FMOD::Sound*			m_SE;
	FMOD::Sound*			m_BGM;
};
