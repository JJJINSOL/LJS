#include "Thread.h"
unsigned int WINAPI Runner(LPVOID param)
{
	Thread* thread = (Thread*)param;
	if (thread != nullptr)
	{
		thread->Run();
		return 1;
	}
	return 0;
}
void Thread:: Create(LPVOID param)
{
	if (m_start != false) return;
	//_beginthreadex - <process.h>
	m_thread = _beginthreadex(nullptr,0,Runner,this,0,&m_id);
	m_start = true;
	m_param = param;
}
bool Thread:: Run()
{
	return false;
}
void Thread:: Detach()
{
	CloseHandle((HANDLE)m_thread);
}
Thread:: Thread()
{
	m_start = false;
	m_param = nullptr;
}
Thread:: ~Thread()
{
	CloseHandle((HANDLE)m_thread);
}