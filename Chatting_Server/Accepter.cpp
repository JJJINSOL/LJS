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
			cout <<server->m_userlist.size() <<"명 접속중" << endl;
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