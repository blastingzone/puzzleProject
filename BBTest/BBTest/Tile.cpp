#include "stdafx.h"
#include "Tile.h"


CTile::CTile(void)
{
}


CTile::~CTile(void)
{
	m_Owner = NOBODY;
	m_Item = NOTHING;
	m_Score = 2;
}

Owner CTile::getOwner(){
	return m_Owner;
}

void CTile::setOwner(Owner inputOwner){
	m_Owner = inputOwner;

	return;
}

Item CTile::getItem(){
	return m_Item;
}

void CTile::setItem(Item inputItem){
	m_Item = inputItem;

	setScore(inputItem);

	return;
}

int	CTile::getScore(){
	return m_Score;
}

void CTile::setScore(Item inputItem){
	switch (inputItem)
	{
	case NOTHING:
		break;
	case GOLD:
		m_Score += 5;
		break;
	case TRASH:
		m_Score += -10;;
		break;
	default:
		break;
	}

	return;
}