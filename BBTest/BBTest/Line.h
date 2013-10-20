#pragma once
#include "mapobj.h"
class CLine :
	public CMapObj
{
public:
	CLine(void);
	~CLine(void);

private :
	bool connected;

public :
	bool isConnected();
	void connect();
};

