#pragma once
#include "User.h"
//#include <WinSock2.h>
//#pragma comment (lib, "ws2_32.lib")
#include <Windows.h>
#include <iostream>
using namespace std;
class Server : public User
{
public:
	SOCKET m_socket;
	bool InitServer(int port);

	Server();
	~Server();
};

