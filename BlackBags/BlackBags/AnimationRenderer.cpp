#include "stdafx.h"
#include "AnimationRenderer.h"
#include "MacroSet.h"
#include <assert.h>
#include <combaseapi.h>

CAnimationRenderer::~CAnimationRenderer(void)
{

}

void CAnimationRenderer::Release()
{
	SafeRelease(m_pImagingFactory);
	SafeRelease(m_pDecoder);
	SafeRelease(m_pFrame);
	SafeRelease(m_pConvertedSourceBitmap);
	SafeRelease(m_ipBitampTraget);
	SafeRelease(m_LoadedBitmap);
}

bool CAnimationRenderer::LoadAnimationImage(float width, float height, float frameSpeed, LoopType loopType)
{
	if ( m_ipRenderTarget == nullptr )
		return false;

	HRESULT hr;

	const WCHAR * myFile = m_fileName.c_str();
	CoInitialize(NULL);
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pImagingFactory)
		);

	if ( SUCCEEDED(hr))
	{
		hr = m_pImagingFactory->CreateDecoderFromFilename(
			myFile,                      // Image to be decoded
			NULL,                            // Do not prefer a particular vendor
			GENERIC_READ,                    // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
			&m_pDecoder                        // Pointer to the decoder
			);
	}
	// Retrieve the first frame of the image from the decoder
	// gif �̹����� ��� ������ ���� �ٲ� �ָ� ���� �������� �ҷ� �� �� �ִ�.
	// ��������Ʈ�� �̰Ŷ� �ٸ�->�߶� ���°�?
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
		//���� �͵��� �����ش�.
		SafeRelease(m_LoadedBitmap);
		hr = m_ipRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_LoadedBitmap);
	}

	assert(hr == S_OK);

	SafeRelease(m_pDecoder);
	SafeRelease(m_pFrame);

	m_AnimationState = S_PAUSE;

	CutFrames(width, height);
	SetFrameSpeed(frameSpeed);
	SetLoop(loopType);

	return true;
}

void CAnimationRenderer::CutFrames(float width, float height)
{
	D2D1_SIZE_F size = m_LoadedBitmap->GetSize();
	float ImageWidth = size.width;
	float ImageHeight = size.height;

	//Frame������ ���� �Է����� ���� ��쿡�� ���.
	if ( m_TotalFrameNumber== -1 )
		m_TotalFrameNumber = (int)(ImageWidth/width)*(ImageHeight/height);

	m_Frame[0].left = 0;
	m_Frame[0].right = width;
	m_Frame[0].top = 0;
	m_Frame[0].bottom = height;

	for ( int i = 1; i < m_TotalFrameNumber ; ++i )
	{
		if ( m_Frame[i-1].right + width < ImageWidth)
		{
			m_Frame[i].left = m_Frame[i-1].left + width;
			m_Frame[i].right = m_Frame[i].left+ width;
			m_Frame[i].top = m_Frame[i-1].top;
			m_Frame[i].bottom = m_Frame[i-1].bottom;
		}
		else
		{
			m_Frame[i].left = 0;
			m_Frame[i].right = width;
			m_Frame[i].top = m_Frame[i-1].top+height;
			m_Frame[i].bottom = m_Frame[i].top+height;
		}	
	}
}


void CAnimationRenderer::StartAnimation(D2D1_RECT_F imagePosition)
{
	if (m_AnimationState == S_PAUSE)
	{
		m_AnimationState = S_PLAY;
		m_CheckedTime = GetTickCount();
	}

	m_CurrentTime = GetTickCount();
	DWORD interval = m_CurrentTime - m_CheckedTime;

	if (m_LoopType == S_LT_ONCE && m_AnimationState == S_PLAY)
	{
		if (interval > m_FrameSpeed && m_CurrentFrame < m_TotalFrameNumber)
		{
			m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,imagePosition,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,m_Frame[m_CurrentFrame++%m_TotalFrameNumber]);
			m_CheckedTime = m_CurrentTime;
		}
		else if (m_CurrentFrame == (m_TotalFrameNumber-1))
		{
			m_AnimationState = S_STOP;
		}
		else
		{
			m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,imagePosition,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,m_Frame[m_CurrentFrame%m_TotalFrameNumber]);
		}
	}

	else if (m_LoopType == S_LT_INFINITE && m_AnimationState == S_PLAY)
	{
		if (interval > m_FrameSpeed)
		{
			m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,imagePosition,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,m_Frame[m_CurrentFrame++%m_TotalFrameNumber]);
			m_CheckedTime = m_CurrentTime;		
		}
		else
		{
			m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,imagePosition,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,m_Frame[m_CurrentFrame%m_TotalFrameNumber]);
		}
	}

	else
	{
		m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,imagePosition,1.0f,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,m_Frame[m_CurrentFrame%m_TotalFrameNumber]);
	}
}

void CAnimationRenderer::RotateImage(D2D1_RECT_F dest)
{
	D2D1_MATRIX_3X2_F matRot = ::D2D1::Matrix3x2F::Rotation( 45.0f, 
		D2D1::Point2F( 50.0f, 50.0f ) );

	m_ipRenderTarget->SetTransform( matRot );                            
	m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,  dest);
}

void CAnimationRenderer::StartRotateAnimation( D2D1_RECT_F imagePosition )
{
	if (m_AnimationState == S_PAUSE)
	{
		m_AnimationState = S_PLAY;
		m_CheckedTime = GetTickCount();
		m_rotateDegree = 0.0f;
	}

	m_CurrentTime = GetTickCount();
	DWORD interval = m_CurrentTime - m_CheckedTime;

	if (interval>m_FrameSpeed)
	{
		m_rotateDegree += 5.0f;
		D2D1_MATRIX_3X2_F matRot = ::D2D1::Matrix3x2F::Rotation( m_rotateDegree, 
			D2D1::Point2F( 50.0f, 50.0f ) );

		m_ipRenderTarget->SetTransform( matRot );                            
		m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,  imagePosition);
		m_CheckedTime = m_CurrentTime;
	}
	else
	{
		m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,  imagePosition);
	}
}

void CAnimationRenderer::MoveAnimation( D2D1_RECT_F startPosition,D2D1_RECT_F endPosition)
{
	float verticalDistance = (endPosition.top-startPosition.top)/(m_FrameSpeed/10);
	float horizontalDistance = (endPosition.left - startPosition.left)/(m_FrameSpeed/10);

	if (m_AnimationState == S_PAUSE)
	{
		m_AnimationState = S_PLAY;
		m_CheckedTime = GetTickCount();
		m_horizontalPoint = 0.0f;
		m_verticalPoint = 0.0f;
	}

	m_CurrentTime = GetTickCount();
	DWORD interval = m_CurrentTime - m_CheckedTime;

	if (interval > m_FrameSpeed && m_horizontalPoint < endPosition.left - startPosition.left)
	{
		m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,
			D2D1::RectF
			(startPosition.left+m_horizontalPoint
			,startPosition.top+m_verticalPoint,
			startPosition.right+m_horizontalPoint,
			startPosition.bottom+m_verticalPoint));

		m_CheckedTime = m_CurrentTime;
		m_horizontalPoint += horizontalDistance;
		m_verticalPoint += verticalDistance;
	}
	else
	{
		m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,
			D2D1::RectF
			(startPosition.left+m_horizontalPoint
			,startPosition.top+m_verticalPoint,
			startPosition.right+m_horizontalPoint,
			startPosition.bottom+m_verticalPoint));
	}
}

void CAnimationRenderer::ScaleAnimation(D2D1_RECT_F imagePosition, float destScale )
{
	if (m_AnimationState == S_PAUSE)
	{
		m_AnimationState = S_PLAY;
		m_CheckedTime = GetTickCount();
		m_CurrentScale = 1.0f;
	}

	if( m_AnimationState == S_PLAY)
	{
		m_CurrentTime = GetTickCount();
		DWORD interval = m_CurrentTime - m_CheckedTime;

		if (interval> m_FrameSpeed && destScale > 1.0f)
		{
			D2D1_MATRIX_3X2_F matScale = ::D2D1::Matrix3x2F::Scale( D2D1::Size(m_CurrentScale,m_CurrentScale), 
				D2D1::Point2F( imagePosition.left+(imagePosition.right-imagePosition.left)/2, imagePosition.top +(imagePosition.bottom-imagePosition.top)/2) );

			m_ipRenderTarget->SetTransform( matScale );                            
			m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,  imagePosition);
			m_CheckedTime = m_CurrentTime;
			m_CurrentScale += 0.05f;

			if (destScale < m_CurrentScale)
				m_AnimationState = S_STOP;
		}
		else if ( interval> m_FrameSpeed && destScale < 1.0f)
		{
			D2D1_MATRIX_3X2_F matScale = ::D2D1::Matrix3x2F::Scale( D2D1::Size(m_CurrentScale,m_CurrentScale), 
				D2D1::Point2F( 50.0f, 50.0f ) );

			m_ipRenderTarget->SetTransform( matScale );                            
			m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,  imagePosition);
			m_CheckedTime = m_CurrentTime;
			m_CurrentScale -= 0.05f;

			if (destScale > m_CurrentScale)
				m_AnimationState = S_STOP;
		}
		else
		{
			m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,  imagePosition);
		}
	}
	else
	{
		m_ipRenderTarget->DrawBitmap(m_LoadedBitmap,  imagePosition);
	}

}
