#pragma once
#include "User.h"
#include "Std_Server.h"
using namespace std;
class Server : public User
{
public:
	list<User*> m_userlist;
	SOCKET m_socket;
	
	//LPVOID m_server;

	virtual bool InitServer(int port);
	virtual bool Run();
	virtual bool Release();
	virtual bool AddUser(SOCKET sock, SOCKADDR_IN clientaddr);
	bool Broadcast(User* user);
	Server();
	~Server();
};

