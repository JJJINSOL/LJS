#include "Accepter.h"
bool Accepter:: Run()
{
	Server* server = (Server*)m_param;
	SOCKET sock = server->m_socket;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		//accpet(����/Ŭ��/ũ��)
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
			cout <<server->m_userlist.size() <<"�� ������" << endl;
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