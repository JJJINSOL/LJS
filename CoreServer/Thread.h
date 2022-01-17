#pragma once
#include "ServerObj.h"
#include <process.h>
class Thread : public ServerObj
{
public:
	//uintptr_t - 포인터 주소 저장하는데 사용
	uintptr_t m_hThread;
	unsigned int m_iID;
	bool m_bStarted;
	LPVOID m_pObject;

	void Create();
	void Create(LPVOID pValue);
	void Join();
	void Detach();
	virtual bool Run();
	static unsigned int WINAPI Runner(LPVOID param);

	Thread();
	Thread(LPVOID pValue);
	virtual ~Thread();
};

