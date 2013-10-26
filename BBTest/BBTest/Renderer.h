#pragma once
#include <d2d1.h>

class CRenderer
{
public:
	CRenderer(void);
	~CRenderer(void);

	bool Init(HWND hwnd);
	bool Release();

	bool Clear();
	bool Begin();
	bool RenderAll();
	bool End();

	ID2D1Factory* GetD2DFactory() const { return m_ipD2DFactory; }
	ID2D1HwndRenderTarget* GetHwndRenderTarget() const { return m_ipRenderTarget; }

private:
	ID2D1Factory*			m_ipD2DFactory;
	ID2D1HwndRenderTarget*	m_ipRenderTarget;
};