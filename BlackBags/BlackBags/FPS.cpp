#include "stdafx.h"
#include "FPS.h"
#include "Renderer.h"

CFPS* CFPS::m_pInstance = nullptr;

// 싱글톤이며 초기화 할 때 필요한 자원들을 할당합니다 (문자를 쓰기 위한 팩토리를 할당하고 글꼴 포맷을 정하고...)
CFPS::CFPS(void)
	: m_FrameAccumulate(0)
	, m_TimeAccumulate(0)
	, m_FPS(0.0f)
	, m_PosX(20.0f)
	, m_PosY(20.0f)
	, m_DWriteFactory(nullptr)
	, m_TextBrush(nullptr)
	, m_TextFormat(nullptr)
{
	DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED
		,__uuidof(m_DWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_DWriteFactory)
		);

	m_DWriteFactory->CreateTextFormat(L"Consolas"
		, NULL
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, 20.0f
		, L"ko"
		, &m_TextFormat);

	// Init 될 때 SystemTime을 한 번 받습니다.
	// 이것 때문에 최초에 한 번 찍히는 FPS는 부정확하지만 크게 중요하지 않습니다. (곧 다음 값이 덮어버리니까요)
	GetSystemTime(&m_PreviousTime);
}


CFPS::~CFPS(void)
{
}

// Brush를 할당하는 함수입니다.
void CFPS::makeBrush()
{
	CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DarkSlateGray)
		, &m_TextBrush);
}

// 이 Update문은 렌더가 한 번 불려올 때 마다 같이 불려옵니다. (즉 1프레임에 1회 실행)
void CFPS::Update()
{
	// FrameAccumulate를 증가시킵니다. 이 값은 다음 FPS 값을 계산하기 전까지 누적된 프레임의 갯수입니다.
	++m_FrameAccumulate;
	// 현재 시스템 시간을 받아옵니다.
	GetSystemTime(&m_CurrentTime);
	// (현재 초 * 1000 + 현재 밀리초) - (과거 초 * 1000 + 과거 밀리초)
	// 왜냐하면 w.Milliseconds 함수는 정말 밀리초 단위만 보여주기 때문입니다. 밀리초만 빼면 오류가 납니다.
	// (예컨대 1초 100밀리초와 0초 0밀리초를 계산하면 차이가 1초 100밀리초가 아니라 100밀리초가 되는 것이죠)
	m_TimeAccumulate = m_CurrentTime.wSecond * 1000 + m_CurrentTime.wMilliseconds
		- m_PreviousTime.wSecond * 1000 - m_PreviousTime.wMilliseconds;

	// 이전 시간과 현재 시간의 차이가 1000 밀리초, 즉 1초가 넘은 시점에
	if ( m_TimeAccumulate > 1000)
	{
		// FPS는 정의대로 Frame / second가 됩니다.
		m_FPS = ( m_FrameAccumulate * 1000.0f ) / ( m_TimeAccumulate );
		swprintf_s(m_Result, L"FPS : %.2f", m_FPS);
		// 다음 FPS 계산을 위해 변수들을 초기화해주고 이전 시간 = 현재 시간으로 만듭니다.
		m_TimeAccumulate = 0;
		m_FrameAccumulate = 0;
		m_PreviousTime = m_CurrentTime;
	}
}

// FPS를 렌더 해 주는 메소드입니다.
void CFPS::Render()
{
	m_Matrix = D2D1::Matrix3x2F::Translation(0, 0);
	CRenderer::GetInstance()->GetHwndRenderTarget()->SetTransform( m_Matrix );

	CRenderer::GetInstance()->GetHwndRenderTarget()->DrawTextW(
		m_Result
		,wcslen(m_Result)
		,m_TextFormat
		,D2D1::RectF(m_PosX, m_PosY
			,CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().width
			,CRenderer::GetInstance()->GetHwndRenderTarget()->GetSize().height)
		,m_TextBrush);
}

// 싱글톤 메소드
CFPS* CFPS::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CFPS();
	}

	return m_pInstance;
}

// Create한 자원들 안 지워줘도 반환된다면서요?
void CFPS::Release()
{
	
}

// 자기 자신을 Release 시키고 삭제합니다.
void CFPS::ReleaseInstance()
{
	m_pInstance->Release();
	delete m_pInstance;
}
