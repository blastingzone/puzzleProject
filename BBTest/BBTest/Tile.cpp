#include "stdafx.h"
#include "Tile.h"


CTile::CTile(void)
{
}


CTile::~CTile(void)
{
	owner = NOBODY;
	item = NOTHING;
	score = 2;
}

Owner CTile::getOwner(){
	return owner;
}

void CTile::setOwner(Owner inputOwner){
	owner = inputOwner;

	return;
}

Item CTile::getItem(){
	return item;
}

void CTile::setItem(Item inputItem){
	item = inputItem;

	setScore(inputItem);

	return;
}

int	CTile::getScore(){
}

void CTile::setScore(Item inputItem){
	switch (inputItem)
	{
	case NOTHING:
		break;
	case GOLD:
		score += 5;
		break;
	case TRASH:
		score += -10;;
		break;
	default:
		break;
	}

	return;
}