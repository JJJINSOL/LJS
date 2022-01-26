#include "Accepter.h"
bool Accepter:: Run()
{
	ChattingServer* server = (ChattingServer*)m_param;
	SOCKET sock = server->m_socket;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		//accpet(서버/클라/크기)
		SOCKET clientsock = accept(sock,(sockaddr*)&clientAddr,&len);
		
		if (clientsock == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				break;
			}
		}
		else
		{
			server->AddUser(clientsock, clientAddr);
		}
		Sleep(1);
	}
	return true;
}
Accepter:: Accepter() 
{
	//Run(server);
}
Accepter:: Accepter(LPVOID value) : Thread(value)
{

}
Accepter:: ~Accepter()
{

}