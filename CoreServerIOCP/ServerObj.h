#pragma once
#include "ServerStd.h"
class ServerObj
{
public:
	CRITICAL_SECTION m_cs;
	HANDLE			 m_hKillEvent;

	ServerObj();
	virtual ~ServerObj();
};

