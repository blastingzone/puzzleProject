#include "stdafx.h"
#include "Timer.h"

CTimer* CTimer::m_pInstance = nullptr;

CTimer::CTimer(void)
{
}


CTimer::~CTimer(void)
{
}

CTimer* CTimer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CTimer();
	}

	return m_pInstance;
}

void CTimer::Init(HWND hwnd)
{
	m_HWND = hwnd;
}

void CTimer::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}

void CTimer::StartTimer(UINT timerID, UINT elapse)
{
	SetTimer(m_HWND, timerID, elapse, NULL);
}

void CTimer::EndTimer(UINT timerID)
{
	KillTimer(m_HWND, timerID);
}