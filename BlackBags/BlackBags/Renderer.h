#pragma once
#include <d2d1.h>

class CRenderer
{
public:
	CRenderer(void);
	~CRenderer(void);

	static CRenderer*		CRenderer::GetInstance();
	static void				ReleaseInstance();

	ID2D1Factory*			GetD2DFactory() const { return m_ipD2DFactory; }
	ID2D1HwndRenderTarget*	GetHwndRenderTarget() const { return m_ipRenderTarget; }

	bool	Init(HWND hwnd);
	bool	Clear();
	bool	Begin();
	bool	End();

	/*	화면 크기나 비례가 바뀔 때 호출되어서 m_DisplayScale의 값을 다시 계산하는 함수 */
	void	SetDisplayScale();
	float	GetDisplayScale() const { return m_DisplayScale; }

private:
	static CRenderer*		m_pInstance; //singleton instance

	ID2D1Factory*			m_ipD2DFactory;
	ID2D1HwndRenderTarget*	m_ipRenderTarget;
	
	/*	현재 화면에 표시될 화면의 크기를 저장하는 변수
		0.0 ~ 1.0의 값을 가지고 DEFAULT 화면 크기에 이 변수의 값을 곱해서 화면에 표시될 크기가 결정된다. */
	float					m_DisplayScale;
};