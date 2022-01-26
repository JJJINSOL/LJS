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