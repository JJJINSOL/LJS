#include "NetUser.h"
#include "Server.h"
int NetUser:: Recv()
{
	//비동기 로드
	OV* ov = new OV(OV::MODE_RECV);
	m_wsaRecvBuffer.len = sizeof(char) * 256;
	m_wsaRecvBuffer.buf = m_szRecv;
	DWORD dwRead;
	DWORD lpFlags = 0;
	BOOL ret = WSARecv(m_sock,&m_wsaRecvBuffer,1,&dwRead,&lpFlags,(WSAOVERLAPPED*)ov,nullptr);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return 0;
}
int NetUser::SendMsg(char* msg, int iSize, WORD type)
{
	// 비동기 로드	
	UPACKET uPacket;//1. 패킷 생성
	uPacket.ph.len = iSize + PACKET_HEADER_SIZE;
	uPacket.ph.type = type;
	memcpy(uPacket.msg, msg, iSize);
	//memcpy(m_szSend, &uPacket, uPacket.ph.len);

	OV* ov = new OV(OV::MODE_SEND);
	m_wsaSendBuffer.len = uPacket.ph.len;
	m_wsaSendBuffer.buf = (char*)&uPacket;

	DWORD dwWrite;
	DWORD lpFlags = 0;
	BOOL ret = WSASend(m_sock,
		&m_wsaSendBuffer,
		1,
		&dwWrite,
		0,
		(WSAOVERLAPPED*)ov,
		nullptr);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return 0;
}
int NetUser::SendMsg(UPACKET& packet)
{
	OV* ov = new OV(OV::MODE_SEND);
	m_wsaSendBuffer.len = packet.ph.len;
	m_wsaSendBuffer.buf = (char*)&packet;

	DWORD dwWrite;
	DWORD lpFlags = 0;
	BOOL ret = WSASend(m_sock,
		&m_wsaSendBuffer,
		1,
		&dwWrite,
		0,
		(WSAOVERLAPPED*)ov,
		nullptr);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
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
void NetUser:: set(SOCKET sock, SOCKADDR_IN addr, Server* pServer)
{
	m_pServer = pServer;
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
	m_writepos += recvbyte;// 버퍼에 이전에 저장된 위치
	m_readpos += recvbyte;// 패킷시작 위치로부터 받은 바이트

	if (m_readpos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* upacket = (UPACKET*)&m_recvbuffer[m_packetpos];
		// 적어도 1개의 패킷은 도착했다.
		if (upacket->ph.len <= m_readpos)
		{
			do
			{
				Packet packet(upacket->ph.type);
				memcpy(&packet.m_upacket, &m_recvbuffer[m_packetpos], upacket->ph.len);
				
				/*if (pPacket->ph.type == PACKET_CHAT_MSG)
				{
					m_packetPool.push_back(tPacket);
				}
				else*/
				{
					XPacket xPacket;
					xPacket.pUser = this;
					xPacket.packet = packet;
					m_pServer->m_packetPool.push_back(xPacket);
				}

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