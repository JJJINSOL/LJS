#pragma once
#include "Thread.h"
class ThreadSend : public Thread
{
public:
	bool Run() override;
	ThreadSend() {};
	ThreadSend(LPVOID value);
	//~ThreadSend() {};
};

