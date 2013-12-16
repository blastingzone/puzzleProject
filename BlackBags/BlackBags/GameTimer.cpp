#include "stdafx.h"
#include "GameTimer.h"
#include "Renderer.h"

#define TimeLimit 20;

CGameTimer* CGameTimer::m_pInstance = nullptr;

CGameTimer::CGameTimer(void)
	: m_TimeRest(0)
	, m_CheckedTime(0)
	, m_PosX(20.0f)
	, m_PosY(40.0f)
	, m_DWriteFactory(nullptr)
	, m_TextBrush(nullptr)
	, m_TextFormat(nullptr)
	, m_Result(L"")
{
}

CGameTimer::~CGameTimer(void)
{
}

CGameTimer* CGameTimer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameTimer();
	}

	return m_pInstance;
}

void CGameTimer::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
	}

	m_pInstance = nullptr;
}

void CGameTimer::SetTimerStart()
{
	m_StartTime = GetTickCount();
	m_CheckedTime = m_StartTime;
	m_TimeRest = TimeLimit;
	m_Result = L"Time Left : " + std::to_wstring(m_TimeRest);
}

void CGameTimer::Update()
{
	m_CurrentTime = GetTickCount();

	DWORD interval = m_CurrentTime - m_CheckedTime;

	if (interval > 1000)
	{
		--m_TimeRest;
		m_CheckedTime = m_CurrentTime;
	}

	if (m_TimeRest <= 0)
	{
		CGameData::GetInstance()->SetPlaySceneTimerFlag();
		SetTimerStart();
	}
}


void CGameTimer::Render()
{
	/*
	// bar 형태로 대체함
	m_Matrix = D2D1::Matrix3x2F::Translation(0, 0);
	CRenderer::GetInstance()->GetHwndRenderTarget()->SetTransform( m_Matrix );

	CRenderer::GetInstance()->GetHwndRenderTarget()->DrawTextW(
		m_Result.c_str()
		,m_Result.length()
		,m_TextFormat
		,D2D1::RectF(m_PosX, m_PosY
		,CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width
		,CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height)
		,m_TextBrush);
	*/
}

bool CGameTimer::Init()
{
	HRESULT hr;

	hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED
		,__uuidof(m_DWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_DWriteFactory)
		);
	assert(SUCCEEDED(hr) );

	hr = m_DWriteFactory->CreateTextFormat(_DEBUG_FONT
		, NULL
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, 20.0f
		, L"ko"
		, &m_TextFormat);
	assert(SUCCEEDED(hr) );

	hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Crimson)
		, &m_TextBrush);
	assert(SUCCEEDED(hr) );

	return true;
}

DWORD CGameTimer::GetRemainTime()
{
	/*
	DWORD now = GetTickCount();
	DWORD progressedTime = now - m_StartTime;
	DWORD remainTime = TIME_LIMIT - progressedTime;
	*/

	DWORD remainTime = TIME_LIMIT - (GetTickCount() - m_StartTime);

	if (remainTime < 0)
	{
		//애니메이션 재생 중
		remainTime = 0;
	}
	else if (remainTime > TIME_LIMIT)
	{
		remainTime = TIME_LIMIT;
	}
	
	return remainTime;
}