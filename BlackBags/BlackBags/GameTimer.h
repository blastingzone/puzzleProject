#pragma once
#include <dwrite.h>
#include <string>
#include <d2d1.h>

class CGameTimer
{
public:
	CGameTimer(void);
	~CGameTimer(void);

	void		MakeBrush();
	void		SetTimerStart();
	void		Update();
	void        Render();
	void		ReleaseInstance();
	void		DrawRandomLine();

	static CGameTimer* CGameTimer::GetInstance();

	static CGameTimer* m_pInstance; //sm9: make private

private:
	// 시간 경과를 계산하기 위한 변수들
	int				m_TimeRest;
	SYSTEMTIME		m_StartTime;
	SYSTEMTIME		m_CurrentTime;

	// Timer 렌더링을 위한 변수들
	IDWriteFactory*			m_DWriteFactory;
	ID2D1SolidColorBrush*	m_TextBrush;
	IDWriteTextFormat*		m_TextFormat;

	float					m_PosX;
	float					m_PosY;

	D2D1::Matrix3x2F		m_Matrix;

	// 렌더할 결과를 담는 변수
	wchar_t m_Result[32]; //SM9: 32같은거는 따로 뺄 것
};

