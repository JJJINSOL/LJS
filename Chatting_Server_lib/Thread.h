#pragma once
#include "Obj_Server.h"
class Thread : public Obj_Server
{
public:
	uintptr_t m_thread;
	unsigned int m_id;
	bool m_start;
	LPVOID m_param;

	//void Create(LPVOID param);

	virtual bool Run();
	//static unsigned int WINAPI Runner(LPVOID param);

	Thread();
	~Thread();
};

