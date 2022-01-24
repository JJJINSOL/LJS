#pragma once
#include "Std_Server.h"
#include "Message.h"
class User : public Message
{
public:
	SOCKET m_sock;
	SOCKADDR_IN m_sockaddr;
	string m_ip;
	short m_port;
	string m_name;

	bool m_connect;

	void UserSet(SOCKET sock, SOCKADDR_IN clientaddr);
	bool Dispatch(DWORD trans, NewOV* ov);
	bool Recv();
};

