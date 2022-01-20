#pragma once
#include "Obj_Server.h"
typedef struct {
	OVERLAPPED ov;
	int type;
}NewOV;
class User : public Obj_Server
{
public:
	SOCKET m_sock;
	SOCKADDR_IN m_sockaddr;
	string m_ip;
	short m_port;
	string m_name;

	bool m_connect;

	NewOV m_recv;
	NewOV m_send;
	void UserSet(SOCKET sock, SOCKADDR_IN clientaddr);
};

