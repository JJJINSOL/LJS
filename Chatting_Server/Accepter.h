#pragma once
#include "Thread.h"
#include "Server.h"
class Accepter : public Thread
{
public:
	bool Run();
	LPVOID m_server;

	Accepter();
	Accepter(LPVOID value);
	~Accepter();
};

