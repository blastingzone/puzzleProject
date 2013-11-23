#include "stdafx.h"
#include "ResultScene.h"
#include "GameResult.h"

CResultScene::CResultScene(void)
{
	m_GameResult = nullptr;

	for (int i = 0; i<MAX_PLAYER_NUM;++i)
	{
		m_PlayerScore[i] = 0;
	}
	m_Winner = MO_NOBODY;

	//테스트용으로 생성자에 일단 추가
	CalculateScore();
	DecideWinner();

	m_GameResult = new CGameResult();
	m_GameResult->Init();

	m_SceneStatus = SC_RESULT;

	AddObject(m_GameResult);
}


CResultScene::~CResultScene(void)
{
	RemoveObject();
	SafeDelete(m_GameResult);
}

void CResultScene::EventHandle(Coordinate mouseCoordinate)
{
	/*	finish button을 클릭하면 메인으로 넘어가고 
		그렇지 않은 경우에는 현재 scene 유지 */

	if (mouseCoordinate.m_PosX > 0)
	{
		CGameData::GetInstance()->SetCurrentScene(SC_OPENING);
	}
	CGameData::GetInstance()->SetCurrentScene(SC_OPENING);
}

void CResultScene::MouseOver(Coordinate mouseCoordinate)
{
	//조심해!!
	//finish button mouse over 기능 구현
	if (mouseCoordinate.m_PosX > 0)
	{

	}
}

void CResultScene::Render()
{
	for (auto iter: m_Object)
	{
		iter->Render();
#ifdef _DEBUG		
		CFPS::GetInstance()->Update();
		CFPS::GetInstance()->Render();
#endif
	}
}

void CResultScene::CalculateScore()
{
	for (int playerId = 0; playerId < CGameData::GetInstance()->GetplayerNumber(); ++playerId)
	{
		int	totalScore = CGameData::GetInstance()->GetPlayerTileNumber(playerId) * 2
							+ CGameData::GetInstance()->GetPlayerGoldNumber(playerId) * 5
							- CGameData::GetInstance()->GetPlayerTrashNumber(playerId) * (-10);
		m_PlayerScore[playerId] = totalScore;
	}
}

void CResultScene::DecideWinner()
{
	int winnerId = 0;
	for (int i = 1; i<CGameData::GetInstance()->GetplayerNumber(); ++i)
	{
		if (m_PlayerScore[i] > m_PlayerScore[i-1])
		{
			winnerId = i;
		}
	}
	m_Winner = static_cast<MO_OWNER>(winnerId);
}
