#include "stdafx.h"
#include "Player.h"
#include "Renderer.h"

CPlayer::CPlayer(void)
{
	m_Id = 0;
	m_PlayerName = L"";

	m_pTileColorBrush = nullptr;
	m_pPlayerFace = nullptr;
	m_pPlayerResultFace = nullptr;
	m_pPlayerResultTile = nullptr;

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
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_1_face.png", m_pPlayerFace);
		m_pPlayerSelectedFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_1_selected_face.png", m_pPlayerSelectedFace);
		m_pPlayerResultFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_face1.png", m_pPlayerResultFace);
		m_pPlayerResultTile = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_tile0.png", m_pPlayerResultTile);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_1_), &m_pTileColorBrush);
		break;
	case MO_PLAYER2:
		SetPlayerName(L"Character2");
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_2_face.png", m_pPlayerFace);
		m_pPlayerSelectedFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_2_selected_face.png", m_pPlayerSelectedFace);
		m_pPlayerResultFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_face2.png", m_pPlayerResultFace);
		m_pPlayerResultTile = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_tile0.png", m_pPlayerResultTile);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_2_), &m_pTileColorBrush);
		break;
	case MO_PLAYER3:
		SetPlayerName(L"Character3");
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_3_face.png", m_pPlayerFace);
		m_pPlayerSelectedFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_3_selected_face.png", m_pPlayerSelectedFace);
		m_pPlayerResultFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_face3.png", m_pPlayerResultFace);
		m_pPlayerResultTile = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_tile0.png", m_pPlayerResultTile);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_3_), &m_pTileColorBrush);
		break;
	case MO_PLAYER4:
		SetPlayerName(L"Character4");
		m_pPlayerFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_4_face.png", m_pPlayerFace);
		m_pPlayerSelectedFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_4_selected_face.png", m_pPlayerSelectedFace);
		m_pPlayerResultFace = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_face4.png", m_pPlayerResultFace);
		m_pPlayerResultTile = CRenderer::GetInstance()->CreateImage(L"Resource/Image/update/PLAYER_tile0.png", m_pPlayerResultTile);
		
		hr = CRenderer::GetInstance()->GetHwndRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(_COLOR_PLAYER_4_), &m_pTileColorBrush);
		break;
	default:
		return false;
	}

	if ( !SUCCEEDED(hr) )
	{
		std::wstring errorText = L"Error Code : ";
		errorText.append( std::to_wstring( GetLastError() ) );
		MessageBox(NULL, errorText.c_str(), L"Error!", MB_ICONERROR|MB_DEFAULT_DESKTOP_ONLY);
		// 비정상 종료
		PostQuitMessage(-1);
	}

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

	return 0; //잘못된 값을 찾으면 0반환해서 스킵
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