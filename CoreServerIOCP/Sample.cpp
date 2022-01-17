#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include "ThreadRecv.h"
#include "ThreadSend.h"
void main()
{
	Server server;
	server.InitServer(10000);

	ThreadRecv tRecv(&server);
	ThreadSend tSend(&server);
	tRecv.Detach();
	tSend.Detach();

	server.Run();
	server.Release();

	//trecv.Join();
	//tsend.Join();
}