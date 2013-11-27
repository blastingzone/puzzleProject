#pragma once

#include <dwrite.h>
#include <string>
#include <d2d1.h>

class CFPS
{
public:
	CFPS(void);
	~CFPS(void);

	bool		Init();

	void		Update();
	void        Render();
	void		ReleaseInstance();

	static CFPS* CFPS::GetInstance();

	static CFPS*		m_pInstance;

private:
	void			Release();

	WORD			m_TimeAccumulate;
	int				m_FrameAccumulate;
	SYSTEMTIME		m_PreviousTime;
	SYSTEMTIME		m_CurrentTime;

	// FPS = ( m_FrameAccumulate ) / ( m_TimeAccumulate )

	double			m_FPS;
	std::wstring	m_Result;

	// FPS �������� ���� ������
	IDWriteFactory*			m_DWriteFactory;
	ID2D1SolidColorBrush*	m_TextBrush;
	IDWriteTextFormat*		m_TextFormat;

	float					m_PosX;
	float					m_PosY;

	D2D1::Matrix3x2F		m_Matrix;
};