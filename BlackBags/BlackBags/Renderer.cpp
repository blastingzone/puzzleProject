#include "stdafx.h"
#include "Renderer.h"

CRenderer* CRenderer::m_pInstance = nullptr;

CRenderer::CRenderer(void)
{
	/* 최초에 생성 될 때 화면 크기는 기본값을 가지므로 m_DisplayScale도 1을 할당한다 */
	m_DisplayScale = 1.0f;
	m_ipD2DFactory = nullptr;
	m_ipRenderTarget = nullptr;

	//sm9: 멤버 변수들 많은데 전부 초기화 안해주네? 클래스의 멤버 변수들은 반드시 초기화 하는 습관을 들이도록..
}


CRenderer::~CRenderer(void)
{
}

CRenderer* CRenderer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CRenderer();
	}

	return m_pInstance;
}

bool CRenderer::Init(HWND hwnd)
{
	HRESULT hr;
	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_ipD2DFactory);
	if (hr != S_OK)
	{
		return false;
	}

	RECT rt;
	GetClientRect(hwnd, &rt);
	D2D1_SIZE_U size = D2D1::SizeU(rt.right - rt.left, rt.bottom - rt.top);

	//일단 프레임률 하락 구간 확인을 위해서 vsync는 끈 상태로 생성
	hr = m_ipD2DFactory->CreateHwndRenderTarget( D2D1::RenderTargetProperties(),
												 D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
												 &m_ipRenderTarget );

	if (hr != S_OK)
	{
		return false;
	}
	
	return true;
}

ID2D1Bitmap* CRenderer::CreateImage( std::wstring fileName , ID2D1Bitmap* myBitmap)
{
	HRESULT hr;

	const WCHAR * myFile = fileName.c_str();
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pImagingFactory)
		);

	hr = m_pImagingFactory->CreateDecoderFromFilename(
		myFile,                      // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&m_pDecoder                        // Pointer to the decoder
		);

	// Retrieve the first frame of the image from the decoder
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
			m_pFrame,                          // Input bitmap to convert
			GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
			WICBitmapDitherTypeNone,         // Specified dither pattern
			NULL,                            // Specify a particular palette 
			0.f,                             // Alpha threshold
			WICBitmapPaletteTypeCustom       // Palette translation type
			);
	}

	if (SUCCEEDED(hr))
	{
		//이전 것들은 지워준다.
		SafeRelease(myBitmap);
		hr = m_ipRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &myBitmap);
	}

	SafeRelease(m_pDecoder);
	SafeRelease(m_pFrame);

	return myBitmap;
}

void CRenderer::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}

bool CRenderer::Clear()
{
	m_ipRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::MintCream));
	
	return true;
}

bool CRenderer::Begin()
{
	m_ipRenderTarget->BeginDraw();

	return true;
}

bool CRenderer::End()
{
	m_ipRenderTarget->EndDraw();

	return true;
}

void CRenderer::SetDisplayScale()
{
	D2D1_SIZE_F tempSize;
	float widthScale, heightScale;
	
	tempSize = m_ipRenderTarget->GetSize();
	
	widthScale = tempSize.width / WINDOW_WIDTH;
	heightScale = tempSize.height / WINDOW_HEIGHT;

	/*	게임 화면이 윈도우 크기를 벗어나는 경우를 방지하기 위해서
		화면의 가로 비율과 세로 비율 중 더 작은 것을 전체 비율로 채택한다. */
	m_DisplayScale = (widthScale < heightScale) ? widthScale : heightScale;

	if (m_DisplayScale > 1)
	{
		m_DisplayScale = 1.0f;
	}
}