#include "User.h"
void User:: UserSet(SOCKET sock, SOCKADDR_IN clientaddr)
{
	m_sock = sock;
	m_sockaddr = clientaddr;
	m_ip = inet_ntoa(clientaddr.sin_addr);
	m_port = ntohs(clientaddr.sin_port);
	m_name = "";
}
bool User::Recv()
{
	m_wsaRecvBuffer.len = sizeof(char) * 256;
	m_wsaRecvBuffer.buf = m_szRecv;
	m_ovrecv.type = 1000;

	//BOOL ret = WSARecv(m_sock, );
	return true;
}
int User:: Dispatch(DWORD trans, NewOV* ov)
{
	return 1;
}