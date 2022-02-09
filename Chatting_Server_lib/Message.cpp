#include "Message.h"
int Message::RecvMsg(char* recvbuffer, int recvbyte)
{
	if (m_writePos + recvbyte >= 2048)
	{
		if (m_readPos > 0)
		{
			//memmove = 메모리 이동 함수 - 목적지, 출발지, 크기
			memmove(&m_recvBuffer[0], &m_recvBuffer[m_packetPos], m_readPos);
		}
		m_packetPos = 0;
		m_writePos = m_readPos;
	}
	memcpy(&m_recvBuffer[m_writePos], recvbuffer, recvbyte);
	m_writePos += recvbyte;
	m_readPos += recvbyte;

	//메시지 내용이 있다면
	if (m_readPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* upacket = (UPACKET*)&m_recvBuffer[m_packetPos];
		while (upacket->p_header.len <= m_readPos)
		{
			Packet packet(upacket->p_header.type);
			memcpy(&packet.m_upacket, &m_recvBuffer[m_packetPos], upacket->p_header.len);
			m_packetPool.push_back(packet);

			m_packetPos += upacket->p_header.len;
			m_readPos -= upacket->p_header.len;
			if (m_readPos < PACKET_HEADER_SIZE)
			{
				break;
			}
			upacket = (UPACKET*)m_recvBuffer[m_packetPos];
		}
	}
	return 1;
}
int Message:: SendMsg(SOCKET sock, UPACKET& packet)
{
	char* msg = (char*)&packet;
	int sendsize = 0;
	while (sendsize < packet.p_header.len)
	{

		int sendbyte = send(sock,&msg[sendsize],packet.p_header.len-sendsize,0);
		if (sendbyte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//비정상적인 종료
				return -1;
			}
		}
		sendsize += sendbyte;
	}
	return sendsize;
}
Message:: Message()
{

}
Message:: ~Message()
{

}