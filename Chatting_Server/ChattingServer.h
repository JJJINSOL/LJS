#pragma once
#include "Server.h"
#include "ChattingUser.h"

#define THREAD_MAX 3
class ChattingServer : public Server
{
public:
	HANDLE m_hThread[THREAD_MAX];
	HANDLE m_hServer;

	bool InitServer(int port) override;
	bool Run() override;
	bool Release() override;
	bool AddUser(SOCKET sock, SOCKADDR_IN clientaddr) override;

	ChattingServer() {};
	~ChattingServer() {};
};

