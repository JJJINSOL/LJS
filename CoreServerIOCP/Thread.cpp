#include "Thread.h"
void Thread:: Create()
{
	if (m_bStarted != false) return;
	//(����Ư��/���� ������,0-�⺻ ũ��/�Լ�/�����忡 ���޵� �Ķ����/�÷���/id)
	m_hThread = _beginthreadex(nullptr, 0,Runner,this,0,&m_iID);
	m_bStarted = true;
}
void Thread:: Create(LPVOID pValue)
{
    if (m_bStarted != false) return;
    m_hThread = _beginthreadex(nullptr, 0,Runner,this,0,&m_iID);
    m_pObject = pValue;
    m_bStarted = true;
}
bool Thread:: Run()
{
	return false;
}
unsigned int WINAPI Thread:: Runner(LPVOID param)
{
	Thread* thread = (Thread*)param;
	if (thread != nullptr)
	{
		thread->Run();
		return 1;
	}
	return 0;
}
void Thread::Join()
{
	WaitForSingleObject((HANDLE)m_hThread, INFINITE);
}
void Thread::Detach()
{
	CloseHandle((HANDLE)m_hThread);
}
Thread:: Thread()
{
	m_bStarted = false;
	m_pObject = nullptr;
}
Thread:: Thread(LPVOID pValue)
{
	m_bStarted = false;
	m_pObject = nullptr;
	Create(pValue);
}
Thread:: ~Thread()
{
	CloseHandle((HANDLE)m_hThread);
}