#include "stdafx.h"
#include "Player.h"
#include "Renderer.h"

CPlayer::CPlayer(void)
{
	m_Id = 0;
	m_PlayerName = L"";
	m_PlayerImage = L"";
	m_PlayerBox = L"";

	m_pTileColorBrush = nullptr;
	m_pPlayerFace = nullptr;
	m_pPlayerBox = nullptr;
	m_pPlayerWaitingBox = nullptr;

	ResetValues();
}


CPlayer::~CPlayer(void)
{
	SafeRelease(m_pTileColorBrush);
}

bool CPlayer::Init(MO_OWNER playerIdx)
{
	HRESULT hr;
	
	switch (playerIdx)
	{
	case MO_PLAYER1:
		SetPlayerName(L"Character1");
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player1.png", m_pPlayerFace);
		m_pPlayerBox = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player1Box.png", m_pPlayerBox);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_1_), &m_pTileColorBrush);
		assert(SUCCEEDED(hr) );
		break;
	case MO_PLAYER2:
		SetPlayerName(L"Character2");
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player2.png", m_pPlayerFace);
		m_pPlayerBox = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player2Box.png", m_pPlayerBox);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_2_), &m_pTileColorBrush);
		assert(SUCCEEDED(hr) );
		break;
	case MO_PLAYER3:
		SetPlayerName(L"Character3");
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player3.png", m_pPlayerFace);
		m_pPlayerBox = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player3Box.png", m_pPlayerBox);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_3_), &m_pTileColorBrush);
		assert(SUCCEEDED(hr) );
		break;
	case MO_PLAYER4:
		SetPlayerName(L"Character4");
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player4.png", m_pPlayerFace);
		m_pPlayerBox = CRenderer::GetInstance()->CreateImage(L"Resource/Image/player4Box.png", m_pPlayerBox);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_4_), &m_pTileColorBrush);
		assert(SUCCEEDED(hr) );
		break;
	default:
		return false;
	}
	m_pPlayerWaitingBox = CRenderer::GetInstance()->CreateImage(L"Resource/Image/playerBox.png", m_pPlayerWaitingBox);

	return true;
}

void CPlayer::ResetValues()
{
	m_isJoinGame = false;
	
	m_Turn = 0;

	m_MyTile = 0;
	m_MyGold = 0;
	m_MyTrash = 0;
}

int CPlayer::GetPlayerItemNumber(MO_ITEM itemType)
{
	switch (itemType)
	{
	case MO_NOTHING:
		return m_MyTile;
		break;
	case MO_GOLD:
		return m_MyGold;
		break;
	case MO_TRASH:
		return m_MyTrash;
		break;
	default:
		break;
	}

	//sm9: 리턴 구문 넣을 것
}

void CPlayer::UpdatePlayerResult(MO_ITEM item)
{
	++m_MyTile;

	switch (item)
	{
	case MO_GOLD:
		++m_MyGold;
		break;
	case MO_TRASH:
		++m_MyTrash;
		break;
	default:
		break;
	}
}