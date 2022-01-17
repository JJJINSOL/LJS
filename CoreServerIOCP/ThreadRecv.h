#pragma once
#include "Thread.h"
class ThreadRecv : public Thread
{
public:
	bool Run() override;
	ThreadRecv() {};
	ThreadRecv(LPVOID value);
	//~ThreadRecv() {};
};

