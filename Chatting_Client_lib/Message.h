#pragma once
#include "Packet.h"
#include "NetStd.h"
class Message
{
public:
	char m_recvBuffer[2048];
	int  m_packetPos;
	int  m_writePos;
	int  m_readPos;

	list<Packet> m_packetPool;

	int ReadMsg(char* recvbuffer, int recvbyte);
};

