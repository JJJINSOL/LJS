#pragma once
#include "Server.h"
#include "ChattingUser.h"

//#include 
#define THREAD_MAX 3
class ChattingServer : public Server
{
public:
	HANDLE m_hThread[THREAD_MAX];
	HANDLE m_hServer;

	bool InitServer(int port) override;
	bool AddUser(SOCKET sock, SOCKADDR_IN clientaddr) override;
};

