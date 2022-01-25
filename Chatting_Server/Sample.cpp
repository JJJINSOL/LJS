#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ChattingServer.h"
#include "Accepter.h"
void main()
{
	ChattingServer server;
	server.InitServer(10000);

	Accepter accept(&server);

	server.Run();
	server.Release();
}