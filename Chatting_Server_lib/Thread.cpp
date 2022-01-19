#include "Thread.h"

//void Thread:: Create(LPVOID param)
//{
//	if (m_start != false) return;
//	//_beginthreadex - <process.h>
//	m_thread = _beginthreadex(nullptr,0,Runner,this,0,&m_id);
//	m_start = true;
//	m_param = param;
//}
bool Thread:: Run()
{
	return false;
}
//unsigned int WINAPI Runner(LPVOID param)
//{
//
//}
Thread:: Thread()
{
	m_start = false;
}
Thread:: ~Thread()
{

}