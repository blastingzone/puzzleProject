#pragma once
#include "BandiVideoLibrary.h"
#include "BandiVideoTexture_DX9.h"
#include <string>

class CVideoRender
{
public:
	CVideoRender(void);
	~CVideoRender(void);

	static CVideoRender*		CVideoRender::GetInstance();
	static void				ReleaseInstance();

	HRESULT Init(HWND hwnd);
	void CleanUp();

	void StartVideo();
	void RenderVideo();

	bool IsVideoEnd();

	static CVideoRender*		m_pInstance; //singleton instance

	CBandiVideoLibrary			m_VideoLibrary;
	CBandiVideoTexture*		m_VideoTexture;
	CBandiVideoDevice*			m_VideoDevice;
	BVL_VIDEO_INFO				m_VideoInfo;
	HWND		m_Hwnd;
	std::string	m_Videopath;

	BVL_STATUS m_Status;
};

