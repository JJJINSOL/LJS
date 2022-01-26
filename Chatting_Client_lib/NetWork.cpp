#include "NetWork.h"
bool NetWork:: Initnetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	return true;
}
bool NetWork:: Initserver(int protocal, int port, const char* ip)
{
	m_sock = socket(AF_INET, protocal, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (ip == nullptr)
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sa.sin_addr.s_addr = inet_addr(ip);
	}

	int a = bind(m_sock, (sockaddr*)&sa, sizeof(sa));
	if (a == SOCKET_ERROR) return false;
	a = listen(m_sock, SOMAXCONN);
	if (a == SOCKET_ERROR) return false;
	return true;
}
bool NetWork:: Closenetwork()
{
	closesocket(m_sock);
	WSACleanup();
	return true;
}
int NetWork:: RecvUser(NetUser& user)
{
	char recvbuffer[1024] = { 0, };
	int recvbyte = recv(user.m_sock, recvbuffer, 1024, 0);
	if (recvbyte == 0)
	{
		return 0;
	}
	if (recvbyte == SOCKET_ERROR)
	{
		return -1;
	}
	user.ReadMsg(recvbuffer, recvbyte);
	return 1;
}