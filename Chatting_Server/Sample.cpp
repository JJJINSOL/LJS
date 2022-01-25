#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ChattingServer.h"
#include "Accepter.h"
void main()
{
	ChattingServer server;
	server.InitServer(10000);

	Accepter accept(&server);
	//accept.Run();

	server.Run();
	server.Release();
}