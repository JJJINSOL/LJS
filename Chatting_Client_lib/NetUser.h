#pragma once
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <iostream>
//#include <string>
//#include <list>
//#include <WinSock2.h>
#include "NetStd.h"
#include "Packet.h"
#include "Message.h"
//#pragma comment (lib, "ws2_32.lib")
//using namespace std;
class NetUser : public Message
{
public:
	SOCKET m_sock;
	SOCKADDR_IN m_addr;
	string m_name;
	short m_port;

	bool m_connect = false;

	
	void set(SOCKET sock, SOCKADDR_IN addr);
	//int DispatchRead(char* recvbuffer, int recvbyte);
};

