#pragma once
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")
#include "User.h"
#include <Windows.h>
#include <iostream>
#include <list>
using namespace std;
class Server : public User
{
public:
	list<User*> m_userlist;
	SOCKET m_socket;
	
	//LPVOID m_server;

	virtual bool InitServer(int port);
	virtual bool AddUser(SOCKET sock, SOCKADDR_IN clientaddr);
	int Broadcast(User* user);
	Server();
	~Server();
};

