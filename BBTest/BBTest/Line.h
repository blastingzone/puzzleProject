#pragma once
#include "mapobj.h"
class CLine :
	public CMapObj
{
public:
	CLine(void);
	~CLine(void);

private :
	bool m_Connected;

public :
	bool isConnected();
	void connect();
};

