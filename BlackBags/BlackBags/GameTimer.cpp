#include "stdafx.h"
#include "GameTimer.h"
#include "Renderer.h"

#define TimeLimit 20;

CGameTimer* CGameTimer::m_pInstance = nullptr;

CGameTimer::CGameTimer(void)
	: m_TimeRest(0)
	, m_PosX(20.0f)
	, m_PosY(40.0f)
	, m_DWriteFactory(nullptr)
	, m_TextBrush(nullptr)
	, m_TextFormat(nullptr)
{
	// �ڿ� �Ҵ�
	DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED
		,__uuidof(m_DWriteFactory)
		, reinterpret_cast<IUnknown**>(&m_DWriteFactory)
		);
	m_DWriteFactory->CreateTextFormat(_DEBUG_FONT
		, NULL
		, DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH_NORMAL
		, 20.0f
		, L"ko"
		, &m_TextFormat);

	MakeBrush();
}

CGameTimer::~CGameTimer(void)
{
}

CGameTimer* CGameTimer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameTimer();
	}

	return m_pInstance;
}

void CGameTimer::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
	}

	m_pInstance = nullptr;
}

void CGameTimer::SetTimerStart()
{
	m_StartTime = GetTickCount();
	m_TimeRest = TimeLimit;
	swprintf_s(m_Result, L"Time Rest : %d", m_TimeRest);
}

// ���� : �ݵ�� SetTimerStart()�� ���� �ҷ��� ������ �����ؾ� ��
void CGameTimer::Update()
{
	m_CurrentTime = GetTickCount();

	DWORD interval = m_CurrentTime - m_StartTime;

	if (interval > 1000)
	{
		--m_TimeRest;
		swprintf_s(m_Result, L"Time Rest : %d", m_TimeRest);
		m_StartTime = m_CurrentTime;
	}

	// ó�� �ð� - ���� �ð��� TimeLimit(���⼭�� 20��)�� �Ѿ�� if�� �۵�
	if (m_TimeRest <= 0)
	{
		CGameData::GetInstance()->SetPlaySceneTimerFlag();
		SetTimerStart();
	}
}

void CGameTimer::MakeBrush()
{
	CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Crimson)
		, &m_TextBrush);
}

void CGameTimer::Render()
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