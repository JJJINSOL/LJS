#include "Packet.h"
Packet::Packet(WORD type)
{
	ZeroMemory(&m_upacket, sizeof(UPACKET));
	m_upacket.p_header.len = PACKET_HEADER_SIZE;
	m_offset = m_upacket.msg;
	m_upacket.p_header.type = type;
}
Packet:: ~Packet()
{

}