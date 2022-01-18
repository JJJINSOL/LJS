#include "Obj_Server.h"
Obj_Server:: Obj_Server()
{
	InitializeCriticalSection(&m_cs);
	m_hKillEvent = CreateEvent(0,TRUE,FALSE,0);
}
Obj_Server:: ~Obj_Server()
{
	DeleteCriticalSection(&m_cs);
	CloseHandle(m_hKillEvent);
}