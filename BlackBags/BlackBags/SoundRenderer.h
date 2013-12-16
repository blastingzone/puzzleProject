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

	void PlayBGM(SOUND_BGMList BGMName);
	void StopBGM();
	void PlaySE_DrawLine();
	void PlaySE_Select();


	FMOD::System* GetSystem() { return m_System; }

private:
	static CSoundRenderer*	m_pInstance;

	FMOD::System*			m_System;
	FMOD::ChannelGroup*		m_ChannelGroup;

	//std::map<std::string, FMOD::Sound*> m_SoundTable;

	FMOD::Channel*			m_BGMChannel;
	FMOD::Channel*			m_SEChannel;

	//조심해
	//임시로 5를 넣어 놓은 것.
	FMOD::Sound*			m_SE_DrawLine;
	FMOD::Sound*			m_SE_Select;
	FMOD::Sound*			m_Main;
	FMOD::Sound*			m_Setting;
	FMOD::Sound*			m_Play;
	FMOD::Sound*			m_Result;
};
