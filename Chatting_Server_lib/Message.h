#pragma once
#include "Std_Server.h"
#include "Packet.h"
#include <ws2def.h>
typedef struct {
	OVERLAPPED ov;
	int type;
}NewOV;
class Message
{
public:
	NewOV m_ovrecv;
	NewOV m_ovsend;
	WSABUF m_wsaRecvBuffer;
	WSABUF m_wsaSendBuffer;
	char m_szRecv[256];
	char m_szSend[256];

	char m_recvBuffer[2048];
	int  m_packetPos = 0;
	int  m_writePos = 0;
	int  m_readPos = 0;

	list<Packet> m_packetPool;


	int SendMsg(SOCKET sock, UPACKET& packet);
	int RecvMsg(char* recvbuffer, int recvbyte);
	
	Message();
	~Message();
};

