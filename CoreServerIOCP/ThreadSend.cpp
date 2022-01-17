#include "ThreadSend.h"
#include "Server.h"
bool ThreadSend:: Run() 
{
	Server* server = (Server*)m_pObject;
	SOCKET sock = server->m_listensock;
	while (1)
	{
		WaitForSingleObject(server->g_Mutex, INFINITE);
		list<NetUser>::iterator iter;
		for (iter = server->g_userlist.begin(); iter != server->g_userlist.end();)
		{
			int ret = server->Broadcast(*iter);
			if (ret <= 0)
			{
				iter = server->g_userlist.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		ReleaseMutex(server->g_Mutex);
		Sleep(1);
	}
	return 1;
}
ThreadSend:: ThreadSend(LPVOID value) :Thread(value)
{};