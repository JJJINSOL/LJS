#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"

DWORD WINAPI RecvThread(LPVOID param)
{
	Server* server = (Server*)param;
	SOCKET sock = server->m_listensock;
	while (1)
	{
		//���ؽ������� waitfor(����Լ�) - release ���̰� �Ӱ豸���� ��!
		WaitForSingleObject(server->g_Mutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;

		//ũ��Ƽ�ü��ǿ����� enter - leave ���̰� �Ӱ豸���� ��!
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

		//Sleep = ����Լ��� ������� ����Ī ����!
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