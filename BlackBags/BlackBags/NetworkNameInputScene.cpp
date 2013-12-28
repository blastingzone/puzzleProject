#include "stdafx.h"
#include "NetworkNameInputScene.h"
#include <dwrite.h>


CNetworkNameInputScene::CNetworkNameInputScene(void)
{
	m_SceneStatus = SC_NETWORK_NAMESETTING;

	m_PlayerName = L"TestText";

	m_pRenderTarget = nullptr;
	m_DWriteFactory = nullptr;

	m_StartPosition.height = 0.f;
	m_StartPosition.width = 0.f;

	m_PlayerNameTextSize = 0.0;
	m_PlayerNameTextFormat = nullptr;
}


CNetworkNameInputScene::~CNetworkNameInputScene(void)
{
}

void CNetworkNameInputScene::GoNextScene()
{
	CGameData::GetInstance()->SetCurrentScene( SC_NETWORK_SETTING );
}

bool CNetworkNameInputScene::Init()
{
	CreateResource();

	return true;
}

void CNetworkNameInputScene::Render()
{
	MSG message;
	GetMessage(&message, NULL, NULL, NULL);

	//////////////////////////////////////////////////////////////////////////
	// ���⼭ ó���ϴϱ� �ӵ��� �� ��;;
	//////////////////////////////////////////////////////////////////////////
	//	if ( GetMessage(&message, NULL, NULL, NULL) > 0 )
	// 	{
	// 		if ( message.message == WM_KEYDOWN )
	// 		{
	// 			switch (message.wParam)
	// 			{
	// 			case VK_BACK:
	// 				m_PlayerName.pop_back();
	// 				break;
	// 			default:
	// 				m_PlayerName.append( KeyMap(message.wParam) );
	// 				break;
	// 			}
	// 		}
	// 	}

	D2D1_RECT_F rectName;

	rectName = D2D1::Rect(m_StartPosition.width, m_StartPosition.height, m_StartPosition.width + 100.0f, m_StartPosition.height + 50.0f);

	m_pRenderTarget->DrawText(
		m_PlayerName.c_str(),
		m_PlayerName.length(),
		m_PlayerNameTextFormat,
		rectName,
		m_pPlayerNameBrush
		);
}

void CNetworkNameInputScene::EventHandle( Coordinate mouseCoordinate )
{
	// �׽�Ʈ : ������ NetworkSettingScene���� �Ѿ�� �����.
	GoNextScene();
}

void CNetworkNameInputScene::MouseOver( Coordinate mouseCoordinate )
{

}

void CNetworkNameInputScene::PlayBGM()
{

}

void CNetworkNameInputScene::StopBGM()
{

}

void CNetworkNameInputScene::ResizeClient()
{

}

void CNetworkNameInputScene::ErrorHandling()
{
	//�� MessageBox �Լ��� �۵����� �ʴ���??
	//�ذ� : MB_DEFAULT_DESKTOP_ONLY�� �߰����ش�!
	std::wstring errorText = L"Error Code : ";
	errorText.append( std::to_wstring( GetLastError() ) );
	MessageBox(NULL, errorText.c_str(), L"Error!", MB_ICONERROR|MB_DEFAULT_DESKTOP_ONLY);
	// ������ ����
	PostQuitMessage(-1);
}

void CNetworkNameInputScene::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();
	}

	//�ϵ��ڵ�! config���� ������
	m_PlayerNameTextSize = 25.0f;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_DWriteFactory)
		);

	if ( SUCCEEDED(hr) )
	{
		hr = m_DWriteFactory->CreateTextFormat(
			_MENU_FONT,
			NULL,
			DWRITE_FONT_WEIGHT_THIN,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_PlayerNameTextSize,
			L"ko",
			&m_PlayerNameTextFormat
			);
	}
	else
	{
		ErrorHandling();
	}

	if ( SUCCEEDED(hr) )
	{
		hr = m_PlayerNameTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	else
	{
		ErrorHandling();
	}

	if ( SUCCEEDED(hr) )
	{
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pPlayerNameBrush);
	}
	else
	{
		ErrorHandling();
	}
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
		return L" ";
	}
}

void CNetworkNameInputScene::InputPlayerName(int wParam)
{
	switch (wParam)
	{
	case VK_BACK:
		m_PlayerName.pop_back();
		break;
	default:
		m_PlayerName.append( KeyMap(wParam) );
		break;
	}
}
