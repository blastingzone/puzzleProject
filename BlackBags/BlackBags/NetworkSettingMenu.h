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

	virtual void Render();

	bool	GetIsMineFlag(int idx)					{ return m_PlayerSelect[idx].m_IsMine; }
	void	SetIsMineFlag(int idx, bool flag)		{ m_PlayerSelect[idx].m_IsMine = flag; }
	void	InitMineFlag();

protected:
	virtual bool				CreateResource();
	void						PollingCharacterData();

	NetworkPlayerSelect			m_PlayerSelect[MAX_PLAYER_NUM];
	
	ID2D1SolidColorBrush*		m_MyCharacterBrush;
};

