#pragma once
#include "ChattingServer.h"
class Accepter : public ChattingServer
{
public:
	bool Run(LPVOID iserver);
	Accepter();
	~Accepter();
};

