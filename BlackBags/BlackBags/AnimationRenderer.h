#pragma once
#include <string>
#include <d2d1.h>
#include <wincodec.h>
#include <array>
#include "Renderer.h"
enum LoopType
{
	S_LT_ONCE,
	S_LT_INFINITE,
};
enum AnimationState
{
	S_PAUSE,
	S_PLAY,
	S_STOP
};
class CAnimationRenderer
{
public:
	CAnimationRenderer(void);
	~CAnimationRenderer(void);

public : CAnimationRenderer(std::wstring fileName)
		 {
			 m_ipD2DFactory = CRenderer::GetInstance()->GetD2DFactory();
			 m_ipRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

			 m_pImagingFactory;
			 m_pDecoder = nullptr;
			 m_pFrame = nullptr;	
			 m_pConvertedSourceBitmap = nullptr;
			 m_ipBitampTraget = nullptr;
			 m_LoadedBitmap = nullptr;

			 m_TotalFrameNumber = -1;
			 m_CurrentFrame = 0;

			 m_fileName = fileName;
		 }

		 void Release();

		// bool SetRenderTarget();

		 //생성후->그림을 준비한다.
		 bool LoadAnimationImage(float width, float height, float frameSpeed, LoopType loopType);

		 //프레임 속도를 정한다. (프레임당 초를 입력)
		 void SetFrameSpeed(float second) { m_FrameSpeed = second*1000;}

		 //원래 이미지를 가로, 세로 길이를 정해 자른다.-> 프레임 개수가 알아서 계산됨
		 void CutFrames(float width, float height);

		 //프레임개수를 직접 정할 경우(옵션)
		 void SetFrameNumber(int frameNumber) { m_TotalFrameNumber = frameNumber; }

		 //애니메이션 재생 타입을 정한다 
		 //	S_LT_ONCE : 한 번만 재생 S_LT_INFINITE: 반복 재생
		 void SetLoop(LoopType looptype) { m_LoopType = looptype;}

		 //imagePosition위치에서 에니메이션을 재생한다.
		 void StartAnimation(D2D1_RECT_F imagePosition);

		 //회전시킨다.
		 void StartRotateAnimation(D2D1_RECT_F imagePosition);

		 //이동시킨다.
		 void MoveAnimation(D2D1_RECT_F startPosition,D2D1_RECT_F endPosition);

		 //크기를 조정한다.
		 void ScaleAnimation(D2D1_RECT_F imagePosition,float destScale);

		 void PauseAnimation() { m_AnimationState = S_PAUSE; }
		 void StopAnimation() { m_AnimationState = S_STOP; }

		 //렌더관련
		 void BeginDraw() {m_ipRenderTarget->BeginDraw();}
		 void ClearDraw() {m_ipRenderTarget->Clear();}
		 void EndDraw() {m_ipRenderTarget->EndDraw();}

		 //회전한다
		 void RotateImage(D2D1_RECT_F dest);


private:
	CAnimationRenderer* m_Sprite;
	std::wstring m_fileName;

	LoopType m_LoopType;
	AnimationState m_AnimationState;

	std::array <D2D1_RECT_F,20> m_Frame;
	int m_TotalFrameNumber;

	ID2D1Factory*			m_ipD2DFactory;
	ID2D1HwndRenderTarget*	m_ipRenderTarget;

	IWICImagingFactory * m_pImagingFactory;
	IWICBitmapDecoder *m_pDecoder;
	IWICBitmapFrameDecode *m_pFrame;	
	IWICFormatConverter* m_pConvertedSourceBitmap;
	ID2D1BitmapRenderTarget* m_ipBitampTraget;
	ID2D1Bitmap *m_LoadedBitmap;

	//시간
	DWORD m_StartTime;
	DWORD m_CurrentTime;
	DWORD m_CheckedTime;
	DWORD m_FrameSpeed;

	int m_CurrentFrame;
	D2D1_RECT_F destRect;

	float m_rotateDegree;
	float m_horizontalPoint;
	float m_verticalPoint;

	float m_CurrentScale;
};

