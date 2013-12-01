#pragma once
#include <d2d1.h>
#include <wincodec.h>

class CImageRenderer
{
public:
	CImageRenderer(void);
	~CImageRenderer(void);

	bool Init(HWND hwnd);

	static CImageRenderer*		CImageRenderer::GetInstance();
	static void				ReleaseInstance();

	//이미지를 생성하고 랜더타겟을 반환한다.
	ID2D1HwndRenderTarget* CreateImage(std::wstring fileName);

	//이미지의 위치, 크기를 입력하면 그림을 뿌려준다.
	void RenderImage(float left,float top,float imgWidth, float imgHeight);

	//이미지 랜더러를 사용하는 씬에서 반드시 불러주세요.
	void CleanUp();

private:
	static CImageRenderer*		m_pInstance; //singleton instance

	HWND m_Hwnd;

	IWICImagingFactory * m_pImagingFactory;
	IWICBitmapDecoder *m_pDecoder;
	IWICBitmapFrameDecode *m_pFrame;	
	IWICFormatConverter* m_pConvertedSourceBitmap;
	ID2D1BitmapRenderTarget* m_ipBitampTraget;
	ID2D1Bitmap *m_pD2DBitmap;

	ID2D1Factory*			m_ipD2DFactory;
	ID2D1HwndRenderTarget*	m_ipRenderTarget;

	int m_Frame;
	D2D1_RECT_F m_Rectangle;
	D2D1_SIZE_U m_size;
	RECT rc;
};

