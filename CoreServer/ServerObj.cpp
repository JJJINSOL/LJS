#include "ServerObj.h"
ServerObj::ServerObj()
{
	InitializeCriticalSection(&m_cs);
	m_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
}
ServerObj:: ~ServerObj()
{
	DeleteCriticalSection(&m_cs);
	CloseHandle(m_hKillEvent);
}