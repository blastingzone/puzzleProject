#pragma once
#include "mapobj.h"

//���߿� ���� ��� �� ���� Ŭ�������� ��� �� ���� �����ؼ� ����� �� �ֵ��� �� ��
enum Owner{
	NOBODY = 0,
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4
};

enum Item{
	NOTHING,
	GOLD,
	TRASH
};

class CTile :
	public CMapObj
{
public:
	CTile(void);
	~CTile(void);

private:
	Owner	m_Owner;
	Item	m_Item;
	int		m_Score;

	void	setScore(Item inputItem);

public:
	Owner	getOwner();
	void	setOwner(Owner inputOwner);
	Item	getItem();
	void	setItem(Item inputItem);
	int		getScore();
};

