#include "ThreadRecv.h"
#include "Server.h"
bool ThreadRecv:: Run()
{
	Server* server = (Server*)m_pObject;
	SOCKET sock = server->m_listensock;

	while (1)
	{
		//���ؽ������� waitfor(����Լ�) - release ���̰� �Ӱ豸���� ��!
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

		//Sleep = ����Լ��� ������� ����Ī ����!
		Sleep(1);
	}
	return 1;
}
ThreadRecv:: ThreadRecv(LPVOID value) :Thread(value)
{}