#pragma once
#include "NetUser.h"
class NetWork
{
public:
	SOCKET m_sock;
	list<NetUser> userlist;
	NetUser m_playuser;

	bool Initnetwork();
	bool Initserver(int protocal, int port, const char* ip = nullptr);
	bool Closenetwork();

	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int SendMsg(SOCKET sock, char* msg, int iSize, WORD type);
	int AddUser(SOCKET sock);
	int RecvUser(NetUser& user);

};

