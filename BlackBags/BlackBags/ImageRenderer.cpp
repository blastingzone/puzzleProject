#include "stdafx.h"
#include "ImageRenderer.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <wincodec.h>


CImageRenderer* CImageRenderer::m_pInstance = nullptr;
CImageRenderer::CImageRenderer(void)
{
	m_pDecoder = nullptr;
	m_pFrame = nullptr;
	m_pImagingFactory = nullptr;
	m_Frame = 5;
}


CImageRenderer::~CImageRenderer(void)
{
}

CImageRenderer* CImageRenderer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CImageRenderer();
	}

	return m_pInstance;
}

void CImageRenderer::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}

bool CImageRenderer::Init(HWND hwnd)
{
	m_Hwnd = hwnd;
	if(m_Hwnd == nullptr	)
		return false;
	return true;
}

ID2D1HwndRenderTarget* CImageRenderer::CreateImage(std::wstring fileName)
{
	HRESULT hr;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pImagingFactory)
		);

	if (SUCCEEDED(hr))
	{
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_ipD2DFactory);
	}

	const WCHAR * myFile = fileName.c_str();

	hr = m_pImagingFactory->CreateDecoderFromFilename(
		myFile,                    
		NULL,                           
		GENERIC_READ,                 
		WICDecodeMetadataCacheOnDemand, 
		&m_pDecoder                      
		);

	// gif 이미지의 경우 프래임 수를 바꿔 주면 다음 프레임을 불러 올 수 있다.
	// 스프라이트는 이거랑 다름->잘라서 쓰는거?
	if (SUCCEEDED(hr))
	{
		hr = m_pDecoder->GetFrame(0, &m_pFrame);
	}

	if (SUCCEEDED(hr))
	{
		SafeRelease(m_pConvertedSourceBitmap);
		hr = m_pImagingFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pConvertedSourceBitmap->Initialize(
			m_pFrame,                       
			GUID_WICPixelFormat32bppPBGRA,   
			WICBitmapDitherTypeNone,       
			NULL,                         
			0.f,                             
			WICBitmapPaletteTypeCustom     
			);
	}

	if(SUCCEEDED(hr)){
		D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();


		//해상도
		renderTargetProperties.dpiX = 72;
		renderTargetProperties.dpiY = 72;

		GetClientRect(m_Hwnd,&rc);
		m_size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = m_ipD2DFactory->CreateHwndRenderTarget(
			renderTargetProperties,
			D2D1::HwndRenderTargetProperties(m_Hwnd, m_size),
			&m_ipRenderTarget
			);
	}

	if (SUCCEEDED(hr))
	{
		//이전 것들은 지워준다.
		SafeRelease(m_pD2DBitmap);
		hr = m_ipRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
	}

	SafeRelease(m_pDecoder);
	SafeRelease(m_pFrame);

	return m_ipRenderTarget;
}

void CImageRenderer::RenderImage(float left,float top,float imgWidth, float imgHeight)
{
	m_ipRenderTarget->BeginDraw();
	m_ipRenderTarget->Clear();

	//준비한 이미지의 사이즈를 가져와서 쓸 수 있다.
	//D2D1_SIZE_F rtSize = m_ipRenderTarget->GetSize();
	//m_Rectangle = D2D1::RectF(left,top,left+imgWidth,top+imgHeight);

	m_ipRenderTarget->DrawBitmap(m_pD2DBitmap,D2D1::RectF(left,top,left+imgWidth,top+imgHeight));
	m_ipRenderTarget->EndDraw();
}

void CImageRenderer::CleanUp()
{
	if(m_ipD2DFactory!=nullptr){
		SafeRelease(m_ipD2DFactory);
	}
	if(m_ipRenderTarget!=nullptr){
		SafeRelease(m_ipRenderTarget);
	}
	if(m_pImagingFactory!=nullptr){
		SafeRelease(m_pImagingFactory);
	}
	if(m_pD2DBitmap!=nullptr){
		SafeRelease(m_pD2DBitmap);
	}
	if(m_pConvertedSourceBitmap!=nullptr){
		SafeRelease(m_pConvertedSourceBitmap);
	}
	if(m_ipBitampTraget!=nullptr){
		SafeRelease(m_ipBitampTraget);
	}
}
