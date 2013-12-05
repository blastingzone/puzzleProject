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

	//main���� HWND�� �������� �Լ�. �ѹ��� ���ָ� �ȴ�.
	HRESULT Init(HWND hwnd);

	//���� ��θ� �־��ֱ⸸ �ϸ� �ȴ�.
	//Init->CreateLibrary->StartVideo ->RenderVideo->StopVideo->CleanUp
	bool CreateLibrary(std::string videopath);
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

