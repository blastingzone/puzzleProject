#pragma once
#include <dwrite.h>
#include <string>
#include <d2d1.h>

class CGameTimer
{
public:
	CGameTimer(void);
	~CGameTimer(void);
	bool		Init();

	static CGameTimer* CGameTimer::GetInstance();
	void		ReleaseInstance();

	void		Update();
	void        Render();

	void		SetTimerStart();

	DWORD		GetTime() { return GetTickCount(); }

private:
	static CGameTimer* m_pInstance;

	// �ð� ����� ����ϱ� ���� ������
	int				m_TimeRest;
	//SYSTEMTIME		m_StartTime;
	//SYSTEMTIME		m_CurrentTime;
	DWORD			m_StartTime;
	DWORD			m_CurrentTime;

	// Timer �������� ���� ������
	IDWriteFactory*			m_DWriteFactory;
	ID2D1SolidColorBrush*	m_TextBrush;
	IDWriteTextFormat*		m_TextFormat;

	float					m_PosX;
	float					m_PosY;

	D2D1::Matrix3x2F		m_Matrix;

	// ������ ����� ��� ����
	std::wstring			m_Result;
};

