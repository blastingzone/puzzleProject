﻿#include "stdafx.h"
#include "Renderer.h"
#include "MacroSet.h"

CRenderer* CRenderer::m_pInstance = nullptr;

CRenderer::CRenderer(void)
{
	m_DisplayScale = 1.0f;
}


CRenderer::~CRenderer(void)
{
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

	//일단 프레임률 하락 구간 확인을 위해서 vsync는 끈 상태로 생성
	hr = m_ipD2DFactory->CreateHwndRenderTarget( D2D1::RenderTargetProperties(),
												 D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
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
	/*
	내부에서 추가로 할당한 자원이 없으므로 소멸되기 전에 반납할 자원도 없음

	SafeRelease(m_ipRenderTarget);
	SafeRelease(m_ipD2DFactory);
	*/
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

void CRenderer::SetDisplayScale()
{
	D2D1_SIZE_F tempSize;
	float widthScale, heightScale;
	
	tempSize = m_ipRenderTarget->GetSize();
	
	widthScale = tempSize.width / WINDOW_WIDTH;
	heightScale = tempSize.height / WINDOW_HEIGHT;

	/*	게임 화면이 윈도우 크기를 벗어나는 경우를 방지하기 위해서
		화면의 가로 비율과 세로 비율 중 더 작은 것을 전체 비율로 채택한다. */
	m_DisplayScale = (widthScale < heightScale) ? widthScale : heightScale;

	if (m_DisplayScale > 1)
	{
		m_DisplayScale = 1.0f;
	}
}