#include "stdafx.h"
#include "NetworkSettingMenu.h"
#include "NetworkManager.h"
#include <dwrite.h>


CNetworkSettingMenu::CNetworkSettingMenu(void)
{
	m_StartPosition.height = 0;
	m_StartPosition.width = 0;
}


CNetworkSettingMenu::~CNetworkSettingMenu(void)
{
}


bool CNetworkSettingMenu::CreateResource()
{
	HRESULT hr;

	if (m_pRenderTarget == nullptr)
	{
		m_pRenderTarget = CRenderer::GetInstance()->GetHwndRenderTarget();

		hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGray), &m_pButtonBrush);
		assert(SUCCEEDED(hr));

		// 네트워크용 내가 선택한 캐릭터 표시
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_MyCharacterBrush);
		}
		else
		{
			ErrorHandling();
		}

		assert(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_1_)), &m_PlayerSelect[0].m_pSelectedBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		/* Player별 마우스 오버 및 선택시 색상 */
		assert(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_1_)), &m_PlayerSelect[0].m_pBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}

		assert(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_2_)), &m_PlayerSelect[1].m_pSelectedBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_2_)), &m_PlayerSelect[1].m_pBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_3_)), &m_PlayerSelect[2].m_pSelectedBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_3_)), &m_PlayerSelect[2].m_pBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_4_)), &m_PlayerSelect[3].m_pSelectedBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(_COLOR_PLAYER_4_)), &m_PlayerSelect[3].m_pBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		/* Player, 맵 버튼에서 마우스 오버 및 선택시 색상 */

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &m_pUnselectedTextBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pSelectedTextBrush);

		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::BlanchedAlmond), &m_pMapBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(245.0f/255,93.0f/255,111.0f/255), &m_pMapSelectedBackgroundBrush);
		}
		else
		{
			ErrorHandling();
		}
		assert(SUCCEEDED(hr));

		if (SUCCEEDED(hr) )
		{
			int i = 0;

			// 캐릭터 초상화 생성

			for (i; i < MAX_PLAYER_NUM; ++i)
			{
				int currentPlayerNum = i + 1;
				std::wstring filepath = L"Resource/Image/player" + std::to_wstring(currentPlayerNum);
				filepath.append(L".png");
				m_pCharacterFace[i] = CRenderer::GetInstance()->CreateImage(filepath, m_pCharacterFace[i]);
			}
		}
		else
		{
			ErrorHandling();
		}

		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_DWriteFactory)
			);
		assert(SUCCEEDED(hr));

		if (!SUCCEEDED(hr) )
			return false;

		CalcStartPosition();
		SetObjectSize();
		RefreshTextSize();
	}

	return true;
}


void CNetworkSettingMenu::Render()
{
	//상자를 먼저 그리고 그 위에 글자를 얹는 식이다
	D2D1_RECT_F		rectElement, textPosition, CharacterPortraitPosition;
	D2D1_POINT_2F	pos;

	//Setting Title을 렌더
	pos.x = m_StartPosition.width - m_SettingTitle.m_LayerWidth;
	pos.y = m_StartPosition.height - m_SettingTitle.m_LayerHeight * 3;

	rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_SettingTitle.m_LayerWidth, pos.y + m_SettingTitle.m_LayerHeight);
	textPosition =  D2D1::Rect( rectElement.left + m_SettingTitleTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

	m_pRenderTarget->DrawText(
		m_SettingTitle.m_Title.c_str(),
		m_SettingTitle.m_Title.length(),
		m_MainTitleTextFormat,
		textPosition,
		m_pUnselectedTextBrush
		);

	// 캐릭터 선택창을 알리는 타이틀을 렌더
	// 위치선정
	pos.x = m_StartPosition.width + (-1) * m_PlayerSelect[0].m_ButtonWidth;
	pos.y = m_StartPosition.height - m_PlayerTitle.m_LayerHeight;

	rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_PlayerTitle.m_LayerWidth, pos.y + m_PlayerTitle.m_LayerHeight);
	textPosition =  D2D1::Rect( rectElement.left + m_PlayerTitleTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

	m_pRenderTarget->DrawText(
		m_PlayerTitle.m_Title.c_str(),
		m_PlayerTitle.m_Title.length(),
		m_SubTitleTextFormat,
		textPosition,
		m_pUnselectedTextBrush
		);

	// 캐릭터 선택창 y 값
	pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE;

	//캐릭터 선택창 렌더
	for (int i = 0; i < MAX_PLAYER_NUM; ++i)
	{
		pos.x = m_StartPosition.width + ( (i - 1) * m_PlayerSelect[i].m_ButtonWidth);

		rectElement = D2D1::Rect( pos.x,
			pos.y,
			pos.x + m_PlayerSelect[i].m_ButtonWidth,
			pos.y + m_PlayerSelect[i].m_ButtonHeight);

		textPosition =  D2D1::Rect( rectElement.left + m_PlayerSelectTextMargin,
			rectElement.top,
			rectElement.right,
			rectElement.bottom);
		// 캐릭터 초상화 부분
		// MouseOver 구현시 아래의 if else 문에 넣어서 조절해준다
		CharacterPortraitPosition = D2D1::Rect ( rectElement.left,
			rectElement.top - m_PortraitHeight,
			rectElement.left + m_PortraitWidth,
			rectElement.top);

		// 마우스가 올라가거나 선택된 상태면 자신이 가진 브러쉬로 자신을 칠함
		if (m_PlayerSelect[i].m_IsMouseOver || m_PlayerSelect[i].m_IsSelected)
		{
			if (m_PlayerSelect[i].m_IsMine)
			{
				m_pRenderTarget->FillRectangle(rectElement, m_MyCharacterBrush);
			}
			else
			{
				m_pRenderTarget->FillRectangle(rectElement, m_PlayerSelect[i].m_pSelectedBackgroundBrush);
			}

			m_pRenderTarget->DrawText(
				m_PlayerSelect[i].m_ButtonText.c_str(),
				m_PlayerSelect[i].m_ButtonText.length(),
				m_PlayerSelectTextFormat,
				textPosition,
				m_pSelectedTextBrush
				);
		}
		else // 아니면 글자만 나옴
		{
			m_pRenderTarget->DrawText(
				m_PlayerSelect[i].m_ButtonText.c_str(),
				m_PlayerSelect[i].m_ButtonText.length(),
				m_PlayerSelectTextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
		// 캐릭터 초상화 포지션은 Character 선택창의 중심으로부터
		// 캐릭터 초상화 렌더
		m_pRenderTarget->DrawBitmap(m_pCharacterFace[i], CharacterPortraitPosition);
	}

	// 맵 선택창을 알리는 타이틀을 렌더
	pos.x = m_StartPosition.width + (-1) * m_MapSelect[0].m_ButtonWidth;
	pos.y = m_StartPosition.height + m_PlayerTitle.m_LayerHeight * (SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE - 2);

	rectElement = D2D1::Rect( pos.x, pos.y, pos.x + m_MapTitle.m_LayerWidth,
		pos.y + m_MapTitle.m_LayerHeight);
	textPosition =  D2D1::Rect( rectElement.left + m_MapTitleTextMargin,
		rectElement.top,
		rectElement.right,
		rectElement.bottom);

	m_pRenderTarget->DrawText(
		m_MapTitle.m_Title.c_str(),
		m_MapTitle.m_Title.length(),
		m_SubTitleTextFormat,
		textPosition,
		m_pUnselectedTextBrush
		);

	// 맵 선택창 y 밑으로 얼마나 내려갈지
	pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE;

	// 맵 선택창 렌더
	for (int j = 0; j < MAX_MAPSIZE_NUM; ++j)
	{
		pos.x = m_StartPosition.width + ( (j - 1) * m_MapSelect[j].m_ButtonWidth);

		rectElement = D2D1::Rect( pos.x,
			pos.y,
			pos.x + m_MapSelect[j].m_ButtonWidth,
			pos.y + m_MapSelect[j].m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_MapSelectTextMargin,
			rectElement.top,
			rectElement.right,
			rectElement.bottom);

		// 맵도 마찬가지로 선택되거나 마우스가 올라가 있으면 색이 변함
		if (m_MapSelect[j].m_IsMouseOver && !m_MapSelect[j].m_IsSelected)
		{
			m_pRenderTarget->FillRectangle(rectElement, m_pMapBackgroundBrush);

			m_pRenderTarget->DrawText(
				m_MapSelect[j].m_ButtonText.c_str(),
				m_MapSelect[j].m_ButtonText.length(),
				m_MapSelectTextFormat,
				textPosition,
				m_pSelectedTextBrush
				);
		}
		if ( m_MapSelect[j].m_IsSelected )
		{
			m_pRenderTarget->FillRectangle(rectElement, m_pMapSelectedBackgroundBrush);

			m_pRenderTarget->DrawText(
				m_MapSelect[j].m_ButtonText.c_str(),
				m_MapSelect[j].m_ButtonText.length(),
				m_MapSelectTextFormat,
				textPosition,
				m_pSelectedTextBrush
				);
		}
		else
		{
			m_pRenderTarget->DrawText(
				m_MapSelect[j].m_ButtonText.c_str(),
				m_MapSelect[j].m_ButtonText.length(),
				m_MapSelectTextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
	}

	//NextButton 조건이 부합하면 렌더. 이 조건이 부합하지 않으면 활성화되지도 않는다(클릭불가)
	if (m_NextButton.m_IsPossible)
	{
		pos.x = m_StartPosition.width + ( (MAX_MAPSIZE_NUM - 2) * m_MapSelect[0].m_ButtonWidth);
		// 밑으로  얼마나 내려갈지
		pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_NEXT_BUTTON_Y_POSITION_SCALE;

		rectElement = D2D1::Rect( pos.x,
			pos.y,
			pos.x + m_NextButton.m_ButtonWidth,
			pos.y + m_NextButton.m_ButtonHeight);
		textPosition =  D2D1::Rect( rectElement.left + m_NextButtonTextMargin, rectElement.top, rectElement.right, rectElement.bottom);

		m_pRenderTarget->FillRectangle(rectElement, m_pMapSelectedBackgroundBrush);

		m_pRenderTarget->DrawText(
			m_NextButton.m_ButtonText.c_str(),
			m_NextButton.m_ButtonText.length(),
			m_NextButtonTextFormat,
			textPosition,
			m_pSelectedTextBrush
			);
	}
}

void CNetworkSettingMenu::InitMineFlag()
{
	for (int i = 0 ; i < MAX_PLAYER_NUM; ++i)
	{
		m_PlayerSelect[i].m_IsMine = false;
	}
}

void CNetworkSettingMenu::PollingCharacterData()
{
	for (int PlayerId = 0; PlayerId < MAX_PLAYER_NUM ; ++PlayerId)
	{
		int CharacterId = CNetworkManager::GetInstance()->GetCharacterId(PlayerId);
		
		m_PlayerSelect[CharacterId].m_IsSelected = false;
		m_PlayerSelect[CharacterId].m_IsMine = false;

		if ( 0 <= CharacterId )
		{
			m_PlayerSelect[CharacterId].m_IsSelected = true;
			if ( PlayerId == CNetworkManager::GetInstance()->GetClientId() )
			{
				m_PlayerSelect[CharacterId].m_IsMine = true;
			}
		}
	}
}
