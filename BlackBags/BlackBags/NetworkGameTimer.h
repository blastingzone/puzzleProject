#pragma once
class CNetworkGameTimer
{
public:
	CNetworkGameTimer(void);
	~CNetworkGameTimer(void);
	
	bool		Init();

	static CNetworkGameTimer* CNetworkGameTimer::GetInstance();
	void		ReleaseInstance();

	void		Update();
	void        Render();

	void		SetTimerStart();

	DWORD		GetTime() { return GetTickCount(); }
	DWORD		GetRemainTime();

private:
	static CNetworkGameTimer* m_pInstance;

	// 시간 경과를 계산하기 위한 변수들
	int					m_TimeRest;
	//SYSTEMTIME		m_StartTime;
	//SYSTEMTIME		m_CurrentTime;
	DWORD				m_StartTime;
	DWORD				m_CurrentTime;
	DWORD				m_CheckedTime;

	float					m_PosX;
	float					m_PosY;

	D2D1::Matrix3x2F		m_Matrix;
};

