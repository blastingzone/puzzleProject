#pragma once
#include "mapobj.h"

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

