#pragma once
#include "Std_Server.h"
class Obj_Server
{
public:
	CRITICAL_SECTION m_cs; //<windows.h>
	HANDLE m_hKillEvent;

	Obj_Server();
	~Obj_Server();
};

