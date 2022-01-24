#include "NetUser.h"
int NetUser:: Recv()
{
	//비동기 로드
	m_wsaRecvBuffer.len = sizeof(char) * 256; //256
	m_wsaRecvBuffer.buf = m_szRecv;
	m_ovRecv.type = 1000;
	DWORD dwRead;
	DWORD lpFlags = 0;
	BOOL ret = WSARecv(m_sock,&m_wsaRecvBuffer,1,&dwRead,&lpFlags,(WSAOVERLAPPED*)&m_ovRecv,nullptr);
	return 0;
}
int NetUser:: Dispatch(DWORD dwTrans, OV* tov)
{
	if (m_connect == false)
	{
		return 0;
	}
	if (tov->type == 1000)
	{
		if (!DispatchRecv(m_szRecv, dwTrans))
		{
		}
		Recv();
	}
	if (tov->type == 2000)
	{
		if (!DispatchSend(dwTrans))
		{
		}
	}
	return 1;
}
void NetUser:: set(SOCKET sock, SOCKADDR_IN addr)
{
	m_connect = true;
	ZeroMemory(m_recvbuffer, sizeof(char) * 2048);
	m_packetpos = 0;
	m_writepos = 0;
	m_readpos = 0;

	m_sock = sock;
	m_addr = addr;
	m_name = inet_ntoa(addr.sin_addr);
	m_port = ntohs(addr.sin_port);
}
int NetUser:: DispatchRecv(char* recvbuffer, int recvbyte)
{
	if (m_writepos + recvbyte >= 2048)
	{
		if (m_readpos > 0)
		{
			//memmove = 메모리 이동 함수 - 목적지, 출발지, 크기
			memmove(&m_recvbuffer[0], &m_recvbuffer[m_packetpos], m_readpos);
		}
		m_packetpos = 0;
		m_writepos = m_readpos;
	}
	memcpy(&m_recvbuffer[m_writepos], recvbuffer, recvbyte);
	m_writepos += recvbyte;
	m_readpos += recvbyte;

	if (m_readpos >= PACKET_HEADER_SIZE)
	{
		UPACKET* upacket = (UPACKET*)&m_recvbuffer[m_packetpos];
		if (upacket->ph.len <= m_readpos)
		{
			do
			{
				Packet packet(upacket->ph.type);
				memcpy(&packet.m_upacket, &m_recvbuffer[m_packetpos], upacket->ph.len);
				m_packetpool.push_back(packet);

				//다음패킷 처리
				m_packetpos += upacket->ph.len;
				m_readpos -= upacket->ph.len;
				if (m_readpos < PACKET_HEADER_SIZE)
				{
					break;
				}
				upacket = (UPACKET*)&m_recvbuffer[m_packetpos];
			} while (upacket->ph.len<=m_readpos);
		}
	}
	return 1;
}

int NetUser:: DispatchSend(DWORD dwTrans)
{
	return 0;
}

bool NetUser:: DisConnect()
{
	closesocket(m_sock);
	return true;
}
NetUser::NetUser()
{

}
NetUser::~NetUser()
{

}