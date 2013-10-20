#include "stdafx.h"
#include "Line.h"


CLine::CLine(void)
{
	connected = false;
}


CLine::~CLine(void)
{
}

bool CLine::isConnected(){
	return connected;
}

void CLine::connect(){
	connected = true;

	return;
}