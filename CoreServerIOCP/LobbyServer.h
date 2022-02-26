#pragma once
#include "Server.h"
#include "ChatUser.h"
#define MAX_WORKER_THREAD 3
class LobbyServer : public Server
{
public:
	HANDLE g_hWorkThread[MAX_WORKER_THREAD];
	HANDLE g_hIOCP;

	virtual void LoginReq(Packet& t, NetUser* user) override;
	virtual void ChatMsg(Packet& t, NetUser* user);

	virtual bool InitServer(int iPort) ;
	virtual bool Run();
	virtual bool Release();
	virtual bool AddUser(SOCKET socr, SOCKADDR_IN clientAddr) ;
};

