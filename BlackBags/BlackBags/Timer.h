#pragma once
class CTimer
{
public:
	CTimer(void);
	~CTimer(void);

	static CTimer*			CTimer::GetInstance();
	static void				ReleaseInstance();

	void Init(HWND hwnd);

	void StartTimer(UINT timerID, UINT elapse);
	void EndTimer(UINT timerID);

private:
	static CTimer*		m_pInstance; //singleton instance

	HWND		m_HWND;		//window handle
	UINT_PTR	m_CurrentTimer;
};

