#include "Accepter.h"
#include "Server.h"
#include <WS2tcpip.h>
bool Accepter:: Run()
{
	Server* server = (Server*)m_pObject;
	SOCKET sock = server->m_listensock;
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		SOCKET clientsock = accept(sock, (sockaddr*)&clientAddr, &len);
		if (clientsock == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "ERRORCODE = " << error << endl;
				break;
			}
		}
		else
		{
			server->AddUser(clientsock, clientAddr);
			cout << server->g_userlist.size() << "명 접속중!" << endl;
		}
		Sleep(1);
	}
	return true;
}
Accepter:: Accepter(LPVOID value) : Thread(value)
{

}