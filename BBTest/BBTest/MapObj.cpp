#include "stdafx.h"
#include "MapObj.h"


CMapObj::CMapObj(void)
{
}


CMapObj::~CMapObj(void)
{
}

bool CMapObj::isConnected(){
	return false;
}

void CMapObj::connect(){
	return;
}

Owner CMapObj::getOwner(){
	return NOBODY;
}

void CMapObj::setOwner(Owner inputOwner){
	return;
}

Item CMapObj::getItem(){
	return NOTHING;
}

void CMapObj::setItem(Item inputItem){
	return;
}

int	 CMapObj::getScore(){
	return 0;
}