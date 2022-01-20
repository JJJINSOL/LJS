#include "LobbyServer.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
	LobbyServer* server = (LobbyServer*)param;
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOverlapped;
	while (1)
	{
		//WAIT_OBJECT_0 - ������ ������Ʈ�� �ñ׳� ���·� �Ǿ���.
		if (WaitForSingleObject(server->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		//IOCP�� ����� �Ϸ� ��⿭�� ���� ����� �ϷḦ ��ٸ���. ���� ��⿭�� �Ϸᰡ ���ٸ�, �� �Լ��� ��⿭�� ����� �Ϸᰡ ���� ������ ����Ѵ�.
		//�Ϸ� ť�� �����Ͱ� ������ �۾�����
		//�����尡 �Ϸ��� �۾��� �ִ��� ���
		//(�ڵ�/���۵� ������ ũ��/key/�۽�,���� �Լ� ȣ��� Overlapped ����ü ���� �ּҸ� ������ ������ �ּ�)
		BOOL bReturn = ::GetQueuedCompletionStatus(server->g_hIOCP, &dwTransfer, &KeyValue, &pOverlapped, 1);

		ChatUser* user = (ChatUser*)KeyValue;
		OV* pov = (OV*)pOverlapped;
		if (bReturn == TRUE && user && pov)
		{
			if(dwTransfer==0&& pov->type==1000)
			{
				user->m_connect = false;
			}
			else
			{
				user->Dispatch(dwTransfer, pov);
			}
		}
		else
		{
			if (GetLastError() != WAIT_TIMEOUT)
			{
				::SetEvent(server->m_hKillEvent);
				break;
			}
		}
	}
	return 1;

}
bool LobbyServer::AddUser(SOCKET sock, SOCKADDR_IN clientAddr)
{
	ChatUser* user = new ChatUser;
	user->set(sock,clientAddr);

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	EnterCriticalSection(&m_cs);
	g_userlist.push_back(user);
	LeaveCriticalSection(&m_cs);

	::CreateIoCompletionPort((HANDLE)sock, g_hIOCP, (ULONG_PTR)user, 0);
	//�̰� ������ ä�� �ȵ�
	user->Recv();

	char ip[INET_ADDRSTRLEN];
	cout << "ip =" << inet_ntop(AF_INET, &(clientAddr.sin_addr),ip, INET_ADDRSTRLEN)
		<< "port =" << ntohs(clientAddr.sin_port)
		<< "  " << std::endl;
	return true;
}
bool LobbyServer::InitServer(int iPort)
{
	Server::InitServer(iPort);
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;
		g_hWorkThread[i] = CreateThread(0,0,WorkerThread,this,0,&id);
	}
	return true;
}
bool LobbyServer:: Run()
{
	while (1)
	{
		EnterCriticalSection(&m_cs);

		for (NetUser* user : g_userlist)
		{
			ChatUser* chatuser = (ChatUser*)user;
			if (chatuser->m_packetpool.size() > 0)
			{
				Broadcast(user);
			}
		}
		list<NetUser*>::iterator iter;
		for (iter = g_userlist.begin(); iter != g_userlist.end();)
		{
			if ((*iter)->m_connect == false)
			{
				(*iter)->DisConnect();
				delete(*iter);
				iter = g_userlist.erase(iter);
			}
			else
			{
				iter++;
			}
		}


		LeaveCriticalSection(&m_cs);
		Sleep(1);
	}
	return true;
}
bool LobbyServer:: Release()
{
	CloseHandle(g_hIOCP);
	Server::Release();
	return true;
}
