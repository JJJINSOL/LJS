#pragma once

#include "NetStd.h"
#include "Packet.h"
#include "Message.h"

class NetUser : public Message
{
public:
	SOCKET m_sock;
	SOCKADDR_IN m_addr;
	string m_name;
	short m_port;

	bool m_connect = false;

	//void set(SOCKET sock, SOCKADDR_IN addr);

};

