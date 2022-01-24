#pragma once
#include "Protocol.h"
class Packet
{
public:
	UPACKET m_upacket;
	char* m_offset;
	Packet(WORD type);
	~Packet();
};

