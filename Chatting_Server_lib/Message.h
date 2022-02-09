#pragma once
#include "Obj_Server.h"
#include "Packet.h"

typedef struct {
	OVERLAPPED ov;
	int type;
}NewOV;
class Message : public Obj_Server
{
public:
	NewOV m_ovrecv;
	NewOV m_ovsend;
	WSABUF m_wsaRecvBuffer;
	WSABUF m_wsaSendBuffer;
	char m_szRecv[256];//256//실제 데이터
	char m_szSend[256];

	char m_recvBuffer[2048];
	int  m_packetPos;
	int  m_writePos;
	int  m_readPos;

	list<Packet> m_packetPool;

public:
	int SendMsg(SOCKET sock, UPACKET& packet);
	int RecvMsg(char* recvbuffer, int recvbyte);
	
	Message();
	~Message();
};

