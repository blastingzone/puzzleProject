#include "stdafx.h"
#include "Line.h"


CLine::CLine(void)
{
	m_Connected = false;
}


CLine::~CLine(void)
{
}

bool CLine::isConnected(){
	return m_Connected;
}

void CLine::connect(){
	m_Connected = true;

	return;
}