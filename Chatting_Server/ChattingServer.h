#pragma once
#include "Server.h"
#include "ChattingUser.h"
class ChattingServer : public Server
{
public:
	HANDLE m_hServer;

	bool InitServer(int port) override;
	bool AddUser(SOCKET sock, SOCKADDR_IN clientaddr) override;
};

