#pragma once
#include <d2d1.h>

class CRenderer
{
public:
	CRenderer(void);
	~CRenderer(void);

	bool Init(HWND hwnd);
	bool Release();
	static bool ReleaseInstance();

	bool Clear();
	bool Begin();
	bool RenderAll();
	bool End();

	static CRenderer*	 CRenderer::GetInstance();

	ID2D1Factory*					GetD2DFactory() const { return m_ipD2DFactory; }
	ID2D1HwndRenderTarget*	GetHwndRenderTarget() const { return m_ipRenderTarget; }

private:
	static CRenderer*		m_pInstance; //singleton instance

	ID2D1Factory*					m_ipD2DFactory;
	ID2D1HwndRenderTarget*	m_ipRenderTarget;
};