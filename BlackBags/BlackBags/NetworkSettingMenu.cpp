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

		// ��Ʈ��ũ�� ���� ������ ĳ���� ǥ��
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
		/* Player�� ���콺 ���� �� ���ý� ���� */
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

		/* Player, �� ��ư���� ���콺 ���� �� ���ý� ���� */

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

			// ĳ���� �ʻ�ȭ ����

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
	//���ڸ� ���� �׸��� �� ���� ���ڸ� ��� ���̴�
	D2D1_RECT_F		rectElement, textPosition, CharacterPortraitPosition;
	D2D1_POINT_2F	pos;

	//Setting Title�� ����
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

	// ĳ���� ����â�� �˸��� Ÿ��Ʋ�� ����
	// ��ġ����
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

	// ĳ���� ����â y ��
	pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_PLAYER_BUTTON_Y_POSITION_SCALE;

	//ĳ���� ����â ����
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
		// ĳ���� �ʻ�ȭ �κ�
		// MouseOver ������ �Ʒ��� if else ���� �־ �������ش�
		CharacterPortraitPosition = D2D1::Rect ( rectElement.left,
			rectElement.top - m_PortraitHeight,
			rectElement.left + m_PortraitWidth,
			rectElement.top);

		// ���콺�� �ö󰡰ų� ���õ� ���¸� �ڽ��� ���� �귯���� �ڽ��� ĥ��
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
		else // �ƴϸ� ���ڸ� ����
		{
			m_pRenderTarget->DrawText(
				m_PlayerSelect[i].m_ButtonText.c_str(),
				m_PlayerSelect[i].m_ButtonText.length(),
				m_PlayerSelectTextFormat,
				textPosition,
				m_pUnselectedTextBrush
				);
		}
		// ĳ���� �ʻ�ȭ �������� Character ����â�� �߽����κ���
		// ĳ���� �ʻ�ȭ ����
		m_pRenderTarget->DrawBitmap(m_pCharacterFace[i], CharacterPortraitPosition);
	}

	// �� ����â�� �˸��� Ÿ��Ʋ�� ����
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

	// �� ����â y ������ �󸶳� ��������
	pos.y = m_StartPosition.height + m_PlayerSelect[0].m_ButtonHeight * SC_S_DEFAULT_MAP_BUTTON_Y_POSITION_SCALE;

	// �� ����â ����
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

		// �ʵ� ���������� ���õǰų� ���콺�� �ö� ������ ���� ����
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

	//NextButton ������ �����ϸ� ����. �� ������ �������� ������ Ȱ��ȭ������ �ʴ´�(Ŭ���Ұ�)
	if (m_NextButton.m_IsPossible)
	{
		pos.x = m_StartPosition.width + ( (MAX_MAPSIZE_NUM - 2) * m_MapSelect[0].m_ButtonWidth);
		// ������  �󸶳� ��������
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
