#include "User.h"
void User:: UserSet(SOCKET sock, SOCKADDR_IN clientaddr)
{
	m_sock = sock;
	m_sockaddr = clientaddr;
	m_ip = inet_ntoa(clientaddr.sin_addr);
	m_port = ntohs(clientaddr.sin_port);
	m_name = "";
}