#pragma once
#include "Thread.h"
class Accepter : public Thread
{
	SOCKET m_listensock;
public:
	virtual bool	Set(int iPort);
	bool			Run() override;

	Accepter();
	Accepter(LPVOID value);
	virtual ~Accepter();
};

