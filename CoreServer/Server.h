#pragma once
#include "NetUser.h"
class Server
{
public:
	list<NetUser> g_userlist;
	CRITICAL_SECTION g_cs;
	HANDLE g_Mutex;
	SOCKET m_listensock;

	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int Broadcast(NetUser& user);
	int RecvUser(NetUser& user);

	bool InitServer(int port);
	bool Run();
	bool Release();
};