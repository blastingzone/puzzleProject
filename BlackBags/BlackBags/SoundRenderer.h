#pragma once
#include <map>

//sm9: 얘네들도 tirdparty 필터에 추가 해 놓으렴..
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

	void PlayBGM(SOUND_BGMList BGMName);
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
	FMOD::Sound*			m_Main;
	FMOD::Sound*			m_Setting;
	FMOD::Sound*			m_Play;
	FMOD::Sound*			m_Result;
};
