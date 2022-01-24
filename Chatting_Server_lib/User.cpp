#include "User.h"
void User:: UserSet(SOCKET sock, SOCKADDR_IN clientaddr)
{
	m_connect = true;
	m_sock = sock;
	m_sockaddr = clientaddr;
	m_ip = inet_ntoa(clientaddr.sin_addr);
	m_port = ntohs(clientaddr.sin_port);
	//m_name = "";

	ZeroMemory(m_recvBuffer, sizeof(char) * 2048);
	m_packetPos = 0;
	m_writePos = 0;
	m_readPos = 0;
}
bool User::Recv()
{
	m_wsaRecvBuffer.len = sizeof(char) * 256;
	m_wsaRecvBuffer.buf = m_szRecv;
	m_ovrecv.type = 1000;

	DWORD dwRead;
	DWORD flag = 0;

	BOOL ret = WSARecv(m_sock, &m_wsaRecvBuffer,1,&dwRead,&flag,(WSAOVERLAPPED*)&m_ovrecv,nullptr);
	return true;
}
bool User:: Dispatch(DWORD trans, NewOV* ov)
{
	if (m_connect == false)
	{
		return false;
	}
	if (ov->type == 1000)
	{
		RecvMsg(m_szRecv,trans);
		Recv();
	}
	return true;
}