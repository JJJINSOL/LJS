#include "Accepter.h"
bool Accepter:: Run(LPVOID iserver)
{
	Server* server = (Server*)iserver;
	SOCKET sock = server->m_socket;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		//accpet(서버/클라/크기)
		SOCKET clientsock = accept(sock,(sockaddr*)&clientAddr,&len);
		if (clientsock == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "ErrorCode = " << error << endl;
				break;
			}
		}
		else
		{
			server->AddUser(clientsock, clientAddr);
			cout << "접속완료" << endl;
		}
		Sleep(1);
	}
	return true;
}
Accepter:: Accepter() 
{
	//Run(server);
}
Accepter:: ~Accepter()
{

}