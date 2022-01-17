#include "ThreadRecv.h"
#include "Server.h"
bool ThreadRecv:: Run()
{
	Server* server = (Server*)m_pObject;
	SOCKET sock = server->m_listensock;

	while (1)
	{
		//뮤텍스에서는 waitfor(대기함수) - release 사이가 임계구역이 됨!
		WaitForSingleObject(server->g_Mutex, INFINITE);
		
		list<NetUser>::iterator iter;
		for (iter = server->g_userlist.begin(); iter != server->g_userlist.end();)
		{
			int ret = server->RecvUser(*iter);
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

		//Sleep = 대기함수가 쓰레드들 스위칭 해줌!
		Sleep(1);
	}
	return 1;
}
ThreadRecv:: ThreadRecv(LPVOID value) :Thread(value)
{}