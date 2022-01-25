#pragma once
#include "Thread.h"
#include "Server.h"
class Accepter : public Thread
{
public:
	bool Run();

	Accepter();
	Accepter(LPVOID value);
	~Accepter();
};

