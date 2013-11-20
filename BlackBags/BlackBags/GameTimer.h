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


private:

	static CGameTimer* m_pInstance;

	// �ð� ����� ����ϱ� ���� ������
	int				m_TimeRest;
	SYSTEMTIME		m_StartTime;
	SYSTEMTIME		m_CurrentTime;

	// Timer �������� ���� ������
	IDWriteFactory*			m_DWriteFactory;
	ID2D1SolidColorBrush*	m_TextBrush;
	IDWriteTextFormat*		m_TextFormat;

	float					m_PosX;
	float					m_PosY;

	D2D1::Matrix3x2F		m_Matrix;

	// ������ ����� ��� ����
	wchar_t m_Result[TIME_LEFT_TEXT_LENGTH];
};

