#pragma once
#include "Obj_Server.h"
class User : public Obj_Server
{
public:
	SOCKET m_sock;
	SOCKADDR_IN m_sockaddr;
	string m_name;
	short m_port;

	void UserSet(SOCKET sock, SOCKADDR_IN clientaddr);
};

