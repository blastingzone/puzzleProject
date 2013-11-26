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
	void				ReleaseInstance();

	//main에서 HWND를 가져오는 함수. 한번만 해주면 된다.
	HRESULT Init(HWND hwnd);

	//비디오 경로를 넣어주기만 하면 된다.
	//Init->CreateLibrary->StartVideo ->RenderVideo->StopVideo->CleanUp
	bool CreateLibrary(std::string videopath); //SM9: const std::string& .... 
	void CleanUp();

	void StartVideo();
	void RenderVideo();
	void StopVideo();

	bool IsVideoEnd();

private:
	static CVideoRender*		m_pInstance; //singleton instance

	CBandiVideoLibrary			m_VideoLibrary;
	CBandiVideoTexture*		m_VideoTexture;
	CBandiVideoDevice*			m_VideoDevice;
	BVL_VIDEO_INFO				m_VideoInfo;

	HWND		m_Hwnd;
	BVL_STATUS m_Status;
};

