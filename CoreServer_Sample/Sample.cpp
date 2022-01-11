#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"

DWORD WINAPI RecvThread(LPVOID param)
{
	Server* server = (Server*)param;
	SOCKET sock = server->m_listensock;
	while (1)
	{
		//뮤텍스에서는 waitfor(대기함수) - release 사이가 임계구역이 됨!
		WaitForSingleObject(server->g_Mutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;

		//크리티컬섹션에서는 enter - leave 사이가 임계구역이 됨!
		//EnterCriticalSection(&g_cs);
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
		//LeaveCriticalSection(&g_cs);

		//Sleep = 대기함수가 쓰레드들 스위칭 해줌!
		Sleep(1);
	}
}
DWORD WINAPI SendThread(LPVOID param)
{
	Server* server = (Server*)param;
	SOCKET sock = server->m_listensock;
	while (1)
	{
		WaitForSingleObject(server->g_Mutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		//EnterCriticalSection(&g_cs);
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
		//LeaveCriticalSection(&g_cs);
		Sleep(1);
	}
}
void main()
{
	Server server;
	server.Init();

	DWORD Threadid;
	HANDLE hThreadRecv = ::CreateThread(0, 0, RecvThread, (LPVOID)&server, 0, &Threadid);
	

	DWORD Threadidsend;
	HANDLE hThreadSend = ::CreateThread(0, 0, SendThread, (LPVOID)&server, 0, &Threadidsend);

	

	server.Run();
	server.Release();
	CloseHandle(hThreadRecv);
	CloseHandle(hThreadSend);

}