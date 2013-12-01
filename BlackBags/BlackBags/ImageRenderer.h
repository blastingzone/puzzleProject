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

	//�̹����� �����ϰ� ����Ÿ���� ��ȯ�Ѵ�.
	ID2D1HwndRenderTarget* CreateImage(std::wstring fileName);

	//�̹����� ��ġ, ũ�⸦ �Է��ϸ� �׸��� �ѷ��ش�.
	void RenderImage(float left,float top,float imgWidth, float imgHeight);

	//�̹��� �������� ����ϴ� ������ �ݵ�� �ҷ��ּ���.
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

