#include "stdafx.h"
#include "NetworkGameTimer.h"

#define TimeLimit 20;

CNetworkGameTimer* CNetworkGameTimer::m_pInstance = nullptr;

CNetworkGameTimer::CNetworkGameTimer(void)
	: m_TimeRest(0)
	, m_CheckedTime(0)
	, m_PosX(20.0f)
	, m_PosY(40.0f)
{
}

CNetworkGameTimer::~CNetworkGameTimer(void)
{
}

CNetworkGameTimer* CNetworkGameTimer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CNetworkGameTimer();
	}

	return m_pInstance;
}

void CNetworkGameTimer::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
	}

	m_pInstance = nullptr;
}

void CNetworkGameTimer::SetTimerStart()
{
	m_StartTime = GetTickCount();
	m_CheckedTime = m_StartTime;
	m_TimeRest = TimeLimit;
}

void CNetworkGameTimer::Update()
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


void CNetworkGameTimer::Render()
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

bool CNetworkGameTimer::Init()
{

	return true;
}

DWORD CNetworkGameTimer::GetRemainTime()
{
	/*
	DWORD now = GetTickCount();
	DWORD progressedTime = now - m_StartTime;
	DWORD remainTime = TIME_LIMIT - progressedTime;
	*/
	DWORD remainTime = TIME_LIMIT - (GetTickCount() - m_StartTime);
	
	if (remainTime > TIME_LIMIT)
	{
		remainTime = TIME_LIMIT;
	}
	else if (remainTime > TIME_LIMIT)
	{
		remainTime = TIME_LIMIT;
	}
	
	return remainTime;
}

