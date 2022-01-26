#pragma once
#include "NetUser.h"
class NetWork
{
public:
	SOCKET m_sock;
	list<NetUser> userlist;
	NetUser m_user;

	bool Initnetwork();
	bool Closenetwork();

	int RecvUser(NetUser& user);

};

