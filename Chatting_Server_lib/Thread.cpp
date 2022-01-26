#include "Thread.h"

DWORD WINAPI Runner(LPVOID param)
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
	DWORD id;
	m_thread = CreateThread(nullptr, 0, Runner, this, 0, &id);
	m_start = true;
	m_param = param;
}
bool Thread:: Run()
{
	return false;
}
void Thread:: Detach()
{
	CloseHandle(m_thread);
}
Thread::Thread(LPVOID pValue)
{
	m_start = false;
	m_param = nullptr;
	Create(pValue);
}
Thread:: ~Thread()
{
	CloseHandle(m_thread);
}