#pragma once
#include "NetStd.h"
#include "Packet.h"
class Message
{
public:
	char m_recvBuffer[2048];
	int  m_packetPos;
	int  m_writePos;
	int  m_readPos;

	list<Packet> m_packetPool;

	int ReadMsg(char* recvbuffer, int recvbyte);
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& upacket);
};

