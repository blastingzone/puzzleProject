#include "stdafx.h"
#include "Renderer.h"
#include "MacroSet.h"

CRenderer* CRenderer::m_pInstance = nullptr;

CRenderer::CRenderer(void)
{
}


CRenderer::~CRenderer(void)
{
	//ReleaseInstance();
	//Release();
}

CRenderer* CRenderer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CRenderer();
	}

	return m_pInstance;
}

bool CRenderer::Init(HWND hwnd)
{
	HRESULT hr;
	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_ipD2DFactory);
	if (hr != S_OK)
	{
		return false;
	}

	RECT rt;
	GetClientRect(hwnd, &rt);
	D2D1_SIZE_U size = D2D1::SizeU(rt.right - rt.left, rt.bottom - rt.top);

	hr = m_ipD2DFactory->CreateHwndRenderTarget( D2D1::RenderTargetProperties(),
												 D2D1::HwndRenderTargetProperties(hwnd, size),
												 &m_ipRenderTarget );

	if (hr != S_OK)
	{
		return false;
	}
	
	return true;
}

bool CRenderer::ReleaseInstance()
{
	m_pInstance->Release();
	delete m_pInstance;

	return true;
}

bool CRenderer::Release()
{
	SafeRelease(m_ipRenderTarget);
	SafeRelease(m_ipD2DFactory);

	return true;
}

bool CRenderer::Clear()
{
	m_ipRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::MintCream));
	
	return true;
}

bool CRenderer::Begin()
{
	m_ipRenderTarget->BeginDraw();

	return true;
}

bool CRenderer::End()
{
	m_ipRenderTarget->EndDraw();

	return true;
}

bool CRenderer::RenderAll()
{
	
	return true;
}