#include "stdafx.h"
#include "NetworkNameInputScene.h"
#include <dwrite.h>


CNetworkNameInputScene::CNetworkNameInputScene(void)
{
	m_SceneStatus = SC_NETWORK_NAMESETTING;

	m_PlayerName = L"Name";
	
	m_StartPosition.height = 0.f;
	m_StartPosition.width = 0.f;

	m_PlayerNameTextSize = 0.0;
	m_PlayerNameTextFormat = nullptr;

	m_TitlePositionH = 0.0;
	m_TitlePositionV = 0.0;
	m_TitleWidth = 0.0;
	m_TitleHeight = 0.0;

	m_ButtonPositionH = 0.0;
	m_ButtonPositionV = 0.0;
	m_ButtonWidth = 0.0;
	m_ButtonHeight = 0.0;

	m_NameTextBoxWidth = 0.0;
	m_NameTextBoxHeight = 0.0;

	m_pTitleImage = nullptr;
	m_pNexthImage = nullptr;
	m_pNextSelectedImage = nullptr;

	m_MouseoverFlag = false;
}


CNetworkNameInputScene::~CNetworkNameInputScene(void)
{
	SafeRelease(m_DWriteFactory);
	SafeRelease(m_PlayerNameTextFormat);

	SafeRelease(m_pTitleImage);
	SafeRelease(m_pNexthImage);
	SafeRelease(m_pNextSelectedImage);
}
void CNetworkNameInputScene::GoNextScene()
{
	CGameData::GetInstance()->SetCurrentScene( SC_NETWORK_SETTING );
}

bool CNetworkNameInputScene::Init()
{
	if (!CreateResource() )
	{
		return false;
	}

	ResizeClient();

	return true;
}

void CNetworkNameInputScene::Render()
{
	MSG message;
	GetMessage(&message, NULL, NULL, NULL);

	//background
	m_pRenderTarget->DrawBitmap(CGameData::GetInstance()->GetBackgroundImage(), D2D1::RectF(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT) );

	D2D1_RECT_F rectPosition;

	// title image
	rectPosition = D2D1::Rect(
		m_StartPosition.width - (m_TitleWidth / 2), 
		m_StartPosition.height - m_TitlePositionV,
		m_StartPosition.width + (m_TitleWidth / 2), 
		m_StartPosition.height - m_TitlePositionV + m_TitleHeight
		);

	m_pRenderTarget->DrawBitmap(m_pTitleImage, rectPosition);

	// title image
	rectPosition = D2D1::Rect(
		m_StartPosition.width - (m_ButtonWidth / 2), 
		m_StartPosition.height + m_ButtonPositionV,
		m_StartPosition.width + (m_ButtonWidth / 2), 
		m_StartPosition.height + m_ButtonPositionV + m_TitleHeight
		);

	if (m_MouseoverFlag)
	{
		m_pRenderTarget->DrawBitmap(m_pNextSelectedImage, rectPosition);
	}
	else
	{
		m_pRenderTarget->DrawBitmap(m_pNexthImage, rectPosition);
	}

	// 이름 입력하는 부분
	rectPosition = D2D1::Rect(
		m_StartPosition.width - (m_NameTextBoxWidth / 2), 
		m_StartPosition.height,
		m_StartPosition.width + (m_NameTextBoxWidth / 2), 
		m_StartPosition.height + m_NameTextBoxHeight
		);

	m_pRenderTarget->DrawText(
		m_PlayerName.c_str(),
		m_PlayerName.length(),
		m_PlayerNameTextFormat,
		rectPosition,
		m_pPlayerNameBrush
		);
}

void CNetworkNameInputScene::EventHandle( Coordinate mouseCoordinate )
{
	if (mouseCoordinate.m_PosX > m_StartPosition.width - (m_ButtonWidth / 2)
		&& mouseCoordinate.m_PosX < m_StartPosition.width + (m_ButtonWidth / 2)
		&& mouseCoordinate.m_PosY > m_StartPosition.height + m_ButtonPositionV
		&& mouseCoordinate.m_PosY < m_StartPosition.height + m_NameTextBoxHeight)
	{
		// 테스트 : 무조건 NetworkSettingScene으로 넘어가게 만든다.
		// 각 클라이언트가 SetMyName으로 지정한 자신의 이름을 서버로 전송
		// 서버는 이 이름들을 종합해서 클라이언트들에게 보낸다
		CGameData::GetInstance()->SetMyName(m_PlayerName);
		GoNextScene();
	}
}

void CNetworkNameInputScene::MouseOver( Coordinate mouseCoordinate )
{
	m_MouseoverFlag = false;

	if (mouseCoordinate.m_PosX > m_StartPosition.width - (m_ButtonWidth / 2)
		&& mouseCoordinate.m_PosX < m_StartPosition.width + (m_ButtonWidth / 2)
		&& mouseCoordinate.m_PosY > m_StartPosition.height + m_ButtonPositionV
		&& mouseCoordinate.m_PosY < m_StartPosition.height + m_NameTextBoxHeight)
	{
		m_MouseoverFlag = true;
	}
}

void CNetworkNameInputScene::PlayBGM()
{

}

void CNetworkNameInputScene::StopBGM()
{

}

void CNetworkNameInputScene::ResizeClient()
{
	//화면 크기 조절
	CalcStartPosition();
	SetObjectSize();
	RefreshTextSize();
}

void CNetworkNameInputScene::SetObjectSize()
{
	float tempScale = CRenderer::GetInstance()->GetDisplayScale();

	m_TitlePositionH = tempScale * SC_NS_DEFAULT_TITLE_POSITION_H;
	m_TitlePositionV = tempScale * SC_NS_DEFAULT_TITLE_POSITION_V;
	m_TitleWidth = tempScale * SC_NS_DEFAULT_TITLE_WIDTH;
	m_TitleHeight = tempScale * SC_NS_DEFAULT_TITLE_HEIGHT;

	m_ButtonPositionH = tempScale * SC_NS_DEFAULT_BUTTON_POSITION_H;
	m_ButtonPositionV = tempScale * SC_NS_DEFAULT_BUTTON_POSITION_V;
	m_ButtonWidth = tempScale * SC_NS_DEFAULT_BUTTON_WIDTH;
	m_ButtonHeight = tempScale * SC_NS_DEFAULT_BUTTON_HEIGHT;

	m_PlayerNameTextSize = tempScale * SC_NS_DEFAULT_TEXT_SIZE;
	m_NameTextBoxWidth = tempScale * SC_NS_DEFAULT_TEXT_WIDTH;
	m_NameTextBoxHeight = tempScale * SC_NS_DEFAULT_TEXT_HEIGHT;
}

void CNetworkNameInputScene::RefreshTextSize()
{
	HRESULT hr;

	SafeRelease(m_PlayerNameTextFormat);

	hr = m_DWriteFactory->CreateTextFormat(
		_UI_FONT,
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_PlayerNameTextSize,
		L"ko",
		&m_PlayerNameTextFormat
		);

	if (!SUCCEEDED(hr) )
		ErrorHandling();

	hr = m_PlayerNameTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	if (!SUCCEEDED(hr) )
		ErrorHandling();

	hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	if (!SUCCEEDED(hr) )
		ErrorHandling();
}

void CNetworkNameInputScene::CalcStartPosition()
{
	// 렌더 기준점은 화면 중점으로
	D2D1_SIZE_F centerPosition = m_pRenderTarget->GetSize();

	m_StartPosition.height = centerPosition.height / 2;
	m_StartPosition.width = centerPosition.width / 2;
}

void CNetworkNameInputScene::ErrorHandling()
{
	//왜 MessageBox 함수가 작동하지 않는지??
	//해결 : MB_DEFAULT_DESKTOP_ONLY를 추가해준다!
	std::wstring errorText = L"Error Code : ";
	errorText.append( std::to_wstring( GetLastError() ) );
	MessageBox(NULL, errorText.c_str(), L"Error!", MB_ICONERROR|MB_DEFAULT_DESKTOP_ONLY);
	// 비정상 종료
	PostQuitMessage(-1);
}

bool CNetworkNameInputScene::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_DWriteFactory)
			);
		
		if (!SUCCEEDED(hr) )
			ErrorHandling();

		SetObjectSize();
		RefreshTextSize();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_RESULT_TEXT_) ), &m_pPlayerNameBrush);
	
		if ( !SUCCEEDED(hr) )
		{
			ErrorHandling();
		}

		m_pTitleImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/NAMESETTING_title.png", m_pTitleImage);
		m_pNexthImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/NAMESETTING_next.png", m_pNexthImage);
		m_pNextSelectedImage = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/NAMESETTING_next_selected.png", m_pNextSelectedImage);
	}

	return true;
}

std::wstring CNetworkNameInputScene::KeyMap(int VirtualKeyCode)
{
	switch(VirtualKeyCode)
	{
	case 0x30:
		return L"0";
	case 0x31:
		return L"1";
	case 0x32:
		return L"2";
	case 0x33:
		return L"3";
	case 0x34:
		return L"4";
	case 0x35:
		return L"5";
	case 0x36:
		return L"6";
	case 0x37:
		return L"7";
	case 0x38:
		return L"8";
	case 0x39:
		return L"9";
	case 0x41:
		return L"A";
	case 0x42:
		return L"B";
	case 0x43:
		return L"C";
	case 0x44:
		return L"D";
	case 0x45:
		return L"E";
	case 0x46:
		return L"F";
	case 0x47:
		return L"G";
	case 0x48:
		return L"H";
	case 0x49:
		return L"I";
	case 0x4A:
		return L"J";
	case 0x4B:
		return L"K";
	case 0x4C:
		return L"L";
	case 0x4D:
		return L"M";
	case 0x4E:
		return L"N";
	case 0x4F:
		return L"O";
	case 0x50:
		return L"P";
	case 0x51:
		return L"Q";
	case 0x52:
		return L"R";
	case 0x53:
		return L"S";
	case 0x54:
		return L"T";
	case 0x55:
		return L"U";
	case 0x56:
		return L"V";
	case 0x57:
		return L"W";
	case 0x58:
		return L"X";
	case 0x59:
		return L"Y";
	case 0x5A:
		return L"Z";

	default:
		return L"_";
	}
}

void CNetworkNameInputScene::InputPlayerName(int wParam)
{
	switch (wParam)
	{
	case VK_BACK:
		if (m_PlayerName.length() > 0)
		{
			m_PlayerName.pop_back();
		}
		break;
	default:
		// 이름 최대 길이는 32글자로 제한
		// 조심해! config로 빼세요
		if (m_PlayerName.length() < SC_NS_TEXT_INPUT_LIMIT)
		{
			m_PlayerName.append( KeyMap(wParam) );
		}
		break;
	}
}
