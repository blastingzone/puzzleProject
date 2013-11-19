////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
/// BandiVideoLibrary 2.1
/// 
/// Copyright(C) 2008-2011 BandiSoft.com All rights reserved.
///
/// Visit http://www.bandisoft.com for more information.
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BandiVideoLibrary.h"

CBandiVideoLibrary::CBandiVideoLibrary()
{
	m_dll = NULL;
	m_pCreateBandiVideo2 = NULL;

	m_bvl = NULL;
}

CBandiVideoLibrary::~CBandiVideoLibrary()
{
	Destroy();
}

// ���̺귯�� �ʱ�ȭ
HRESULT CBandiVideoLibrary::Create(LPCTSTR dll_path_name, IBandiVideoFileReader *reader /*= NULL */, IBandiVideoSound *sound /*= NULL */)
{
	if(m_dll) {ASSERT(0); return BVERR_ALREADY_CREATED;}

	// check os version
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osv)==FALSE || 
		osv.dwPlatformId <=VER_PLATFORM_WIN32_WINDOWS)		// does not support 98
	{
		return BVERR_UNSUPPORTED_OS;
	}

	// DLL �ε�
	m_dll = LoadLibrary(dll_path_name);
	if(m_dll==NULL)
	{
		m_dll = LoadLibrary(BANDIVIDEO_RELEASE_DLL_FILE_NAME);
		if(m_dll==NULL)
		{
			ASSERT(0);
			return BVERR_LOAD_LIBRARY_FAIL;
		}
	}

	m_pCreateBandiVideo2 = (LPCREATEBANDIVIDEO2)GetProcAddress(m_dll, "CreateBandiVideo2");
	if(m_pCreateBandiVideo2==NULL)
	{
		ASSERT(0);
		FreeLibrary(m_dll);
		m_dll = NULL;
		return BVERR_GET_PROC_ADDRESS_FAIL;
	}

	return m_pCreateBandiVideo2(BVL_VERSION, (void**)&m_bvl, reader, sound);								
}


BOOL CBandiVideoLibrary::IsCreated()
{
	return m_bvl ? TRUE : FALSE;
}


// ���̺귯�� ����
void CBandiVideoLibrary::Destroy()
{
	if(m_bvl)
	{ 
		m_bvl->Release();
		m_bvl = NULL;
	}

	m_pCreateBandiVideo2 = NULL;
	if(m_dll) FreeLibrary(m_dll);
	m_dll = NULL;
}

// �����ϱ�			
HRESULT	CBandiVideoLibrary::Verify(LPCSTR id, LPCSTR key)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->Verify(id, key);
}

// ���� ����
INT		CBandiVideoLibrary::GetVersion()
{
	if(m_bvl==NULL) {ASSERT(0); return 0;}
	return m_bvl->GetVersion();
}

HRESULT	CBandiVideoLibrary::Open(LPCWSTR path_name, BOOL async)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->Open(path_name, async);
}

HRESULT CBandiVideoLibrary::Open(LPCSTR path_name, BOOL async)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	
	WCHAR wszPath[MAX_PATH+1];
	MultiByteToWideChar(CP_ACP, NULL, path_name, -1, wszPath, MAX_PATH);
	
	return m_bvl->Open(wszPath, async);
}

HRESULT CBandiVideoLibrary::Close()
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->Close();
}

HRESULT CBandiVideoLibrary::Play()
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->Play();
}

HRESULT CBandiVideoLibrary::Stop()
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->Stop();
}

HRESULT	CBandiVideoLibrary::Seek(INT64 value, BVL_SEEK_FLAG seek_flag)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->Seek(value, seek_flag);
}

HRESULT	CBandiVideoLibrary::Pause(BOOL pause)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->Pause(pause);
}

BOOL	CBandiVideoLibrary::IsNextFrame()
{
	if(m_bvl==NULL) {ASSERT(0); return FALSE;}
	return m_bvl->IsNextFrame();
}

HRESULT	CBandiVideoLibrary::GetFrame(BVL_FRAME &frame, BOOL can_skip)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->GetFrame(frame, can_skip);
}

HRESULT	CBandiVideoLibrary::GetVideoInfo(BVL_VIDEO_INFO &info)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->GetVideoInfo(info);
}

HRESULT CBandiVideoLibrary::GetStatus(BVL_STATUS &status)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->GetStatus(status);
}

HRESULT CBandiVideoLibrary::SetVolume(INT volume)
{
	if(m_bvl==NULL) {ASSERT(0); return BVERR_LIBRARY_NOT_LOADED;}
	return m_bvl->SetVolume(volume);
}
