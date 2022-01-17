#pragma once
#include "Thread.h"
class Accepter : public Thread
{
public:
	bool Run() override;
	Accepter() {};
	Accepter(LPVOID value);
	//~Accepter() {};
};
