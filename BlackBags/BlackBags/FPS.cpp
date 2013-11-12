#include "stdafx.h"
#include "FPS.h"
#include "Renderer.h"
#include "MacroSet.h"

CFPS* CFPS::m_pInstance = nullptr;

CFPS::CFPS(void)
	: m_FrameAccumulate(0)
	, m_TimeAccumulate(0)
	, m_FPS(0.0f)
	, m_PosX(20.0f)
	, m_PosY(20.0f)
	, m_DWriteFactory(nullptr)
	, m_TextBrush(nullptr)
	, m_TextFormat(nullptr)
{
	DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED
		,__uuidof(m_DWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_DWriteFactory)
		);

	m_DWriteFactory->CreateTextFormat(L"Consolas"
		, NULL
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, 20.0f
		, L"ko"
		, &m_TextFormat);

	GetSystemTime(&m_PreviousTime);
}


CFPS::~CFPS(void)
{
}

void CFPS::makeBrush()
{
	CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DarkSlateGray)
		, &m_TextBrush);
}

void CFPS::Update()
{
	++m_FrameAccumulate;
	GetSystemTime(&m_CurrentTime);
	m_TimeAccumulate = m_CurrentTime.wSecond * 1000 + m_CurrentTime.wMilliseconds
		- m_PreviousTime.wSecond * 1000 - m_PreviousTime.wMilliseconds;

	if ( m_TimeAccumulate > 1000)
	{
		m_FPS = ( m_FrameAccumulate * 1000.0f ) / ( m_TimeAccumulate );
		swprintf_s(m_Result, L"FPS : %.2f", m_FPS);
		m_TimeAccumulate = 0;
		m_FrameAccumulate = 0;
		m_PreviousTime = m_CurrentTime;
	}
}

void CFPS::Render()
{
	m_Matrix = D2D1::Matrix3x2F::Translation(0, 0);
	CRenderer::GetInstance()->GetHwndRenderTarget()->SetTransform( m_Matrix );

	CRenderer::GetInstance()->GetHwndRenderTarget()->DrawTextW(
		m_Result
		,wcslen(m_Result)
		,m_TextFormat
		,D2D1::RectF(m_PosX, m_PosY
			,CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width
			,CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height)
		,m_TextBrush);
}

CFPS* CFPS::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CFPS();
	}

	return m_pInstance;
}

void CFPS::Release()
{
	SafeRelease(m_TextBrush);
	SafeRelease(m_TextFormat);
	SafeRelease(m_DWriteFactory);
}

void CFPS::ReleaseInstance()
{
	m_pInstance->Release();
	delete m_pInstance;
}
