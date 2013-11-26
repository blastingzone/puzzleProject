#include "stdafx.h"
#include "VideoRender.h"
/*#include "config.h"*/

CVideoRender* CVideoRender::m_pInstance = nullptr;

CVideoRender::CVideoRender(void)
{
	m_VideoDevice=nullptr;
	m_VideoTexture = nullptr;
	m_Hwnd=NULL;

	//SM9: 모든 멤버 변수 초기화 하는 습관
}


CVideoRender::~CVideoRender(void)
{
}

CVideoRender* CVideoRender::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CVideoRender();
	}

	return m_pInstance;
}

void CVideoRender::ReleaseInstance()
{
	CleanUp();
	SafeDelete(m_pInstance);
}

HRESULT CVideoRender::Init(HWND hwnd)
{
	m_Hwnd = hwnd;

	if(m_Hwnd != nullptr)
		return S_OK;
}

void CVideoRender::CleanUp()
{
	m_VideoLibrary.Destroy();

	if (m_VideoTexture != nullptr)
	{
		m_VideoTexture->Close();
		delete m_VideoTexture;
		m_VideoTexture = nullptr;
	}

	if (m_VideoDevice != nullptr)
	{
		m_VideoDevice->Close();
		delete m_VideoDevice;
		m_VideoDevice = nullptr;
	}
}


void CVideoRender::StartVideo()
{
	//BVL_STATUS status;
	m_VideoLibrary.GetStatus(m_Status);

	if (m_Status == BVL_STATUS_READY)
	{
		m_VideoLibrary.GetVideoInfo( m_VideoInfo );
		m_VideoLibrary.Play();
	}
}



void CVideoRender::RenderVideo()
{
	m_VideoLibrary.GetStatus(m_Status);

	if (m_Status == BVL_STATUS_PLAYEND)
	{
		CleanUp();
		return;
	}

	if (m_VideoLibrary.IsNextFrame())
	{
		INT		pitch;
		BYTE*	buf = m_VideoTexture->Lock(pitch);
		if (buf)
		{
			// Get frame
			BVL_FRAME frame;
			frame.frame_buf = buf;
			frame.frame_buf_size = m_VideoInfo.height*pitch;
			frame.pitch = pitch;
			frame.width = m_VideoInfo.width;
			frame.height = m_VideoInfo.height;
			frame.pixel_format = m_VideoTexture->GetFormat();

			m_VideoLibrary.GetFrame(frame, TRUE);

			m_VideoTexture->Unlock();

			// Show frame
			m_VideoDevice->StartFrame();
			m_VideoTexture->Draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			m_VideoDevice->EndFrame();
		}
	}
	else
	{
		Sleep(1);
	}
}

bool CVideoRender::IsVideoEnd()
{
	if (m_Status == BVL_STATUS_PLAYEND)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CVideoRender::CreateLibrary( std::string videopath )
{
	if (FAILED(m_VideoLibrary.Create(BANDIVIDEO_DLL_FILE_NAME, NULL, NULL)))
	{
		return FALSE;
	}

	if (FAILED(m_VideoLibrary.Open(videopath.c_str(), FALSE)))
	{
		return FALSE;
	}

	//BVL_VIDEO_INFO info;
	if (FAILED(m_VideoLibrary.GetVideoInfo(m_VideoInfo)))
	{
		return FALSE;
	}

	m_VideoDevice = new CBandiVideoDevice_DX9();
	if (!m_VideoDevice || FAILED(m_VideoDevice->Open(m_Hwnd)))
	{
		if (m_VideoDevice) delete m_VideoDevice;
		return FALSE;
	}

	m_VideoTexture = new CBandiVideoTexture_DX9((CBandiVideoDevice_DX9*)m_VideoDevice);
	//if (!m_VideoTexture || FAILED(m_VideoTexture->Open(info.width , info.height)))
	if (!m_VideoTexture || FAILED(m_VideoTexture->Open(m_VideoInfo.width , m_VideoInfo.height)))
	{
		if (m_VideoDevice) delete m_VideoDevice;
		if (m_VideoTexture) delete m_VideoTexture;
		return FALSE;
	}

	m_VideoLibrary.Play();

	return TRUE;
}

void CVideoRender::StopVideo()
{
	m_VideoLibrary.Stop();
}

