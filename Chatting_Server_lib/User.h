#pragma once
#include "Obj_Server.h"
#include "Message.h"
class User : public Obj_Server,public Message
{
public:
	SOCKET m_sock;
	SOCKADDR_IN m_sockaddr;
	string m_ip;
	short m_port;
	string m_name;

	bool m_connect;

	void UserSet(SOCKET sock, SOCKADDR_IN clientaddr);
	int Dispatch(DWORD trans, NewOV* ov);
	bool Recv();
};

