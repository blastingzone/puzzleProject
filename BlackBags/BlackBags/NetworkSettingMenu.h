#pragma once
#include "settingmenu.h"

struct NetworkPlayerSelect : public PlayerSelect
{
	NetworkPlayerSelect() : m_IsMine(false) {}

	bool m_IsMine;
};

class CNetworkSettingMenu :
	public CSettingMenu
{
public:
	CNetworkSettingMenu(void);
	virtual ~CNetworkSettingMenu(void);

	virtual void				Render();
	virtual	void				ResizeClient();

	virtual D2D1_SIZE_F GetStartPosition()					{ return m_StartPosition; }
	virtual D2D1_SIZE_F GetPlayerSelectButtonSize();
	virtual D2D1_SIZE_F GetMapSelectButtonSize();
	virtual D2D1_SIZE_F GetNextButtonSize();

	virtual void SetPlayerMouseOver(int idx);
	virtual void SetMapMouseOver(int idx);
	virtual void InitMouseOver();

	virtual void InitMapSelected();

	bool	GetIsMineFlag(int idx)					{ return m_PlayerSelect[idx].m_IsMine; }
	void	SetIsMineFlag(int idx, bool flag)		{ m_PlayerSelect[idx].m_IsMine = flag; }
	void	InitMineFlag();
	void	PollingCharacterData();

protected:
	virtual bool				CreateResource();
	virtual void				CalcStartPosition();
	virtual void				RefreshTextSize();
	virtual void				SetObjectSize();


	NetworkPlayerSelect			m_PlayerSelect[MAX_PLAYER_NUM];
	
	ID2D1SolidColorBrush*		m_MyCharacterBrush;
	
	D2D1_SIZE_F					m_StartPosition;
};

