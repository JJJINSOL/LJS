#pragma once
#include "Obj_Server.h"
class Thread : public Obj_Server
{
public:
	HANDLE m_thread;
	//DWORD m_id;
	bool m_start;
	LPVOID m_param;

	void Create(LPVOID param);
	void Detach();
	virtual bool Run();

	Thread(LPVOID pValue);
	~Thread();
};

