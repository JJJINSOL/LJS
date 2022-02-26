#include "Server.h"
void Server::LoginReq(Packet& t, NetUser* user)
{

}
bool Server::InitServer(int port)
{
	m_fnExecutePacket[PACKET_LOGIN_REQ] = std::bind(&Server::LoginReq, this,
										  std::placeholders::_1,
										  std::placeholders::_2);

	m_Accept.Set(port);
	m_Accept.Create(this);
	m_Accept.Detach();
	return true;
}
bool Server:: Run()
{
	return true;
}
bool Server:: AddUser(SOCKET socr, SOCKADDR_IN clientAddr)
{
	return true;
}
bool Server::DelUser(SOCKET sock)
{
	return true;
}
bool Server::DelUser(NetUser* pUser)
{
	pUser->DisConnect();
	return true;
}
bool Server::DelUser(m_UserIter& iter)
{
	DelUser((NetUser*)*iter);
	return true;
}
bool Server:: Release()
{
	ObjectPool<NetUser>::AllFree();
	ObjectPool<OV>::AllFree();
	return true;
}
int Server::SendMsg(SOCKET sock, UPACKET& packet)
{
	char* msg = (char*)&packet;
	int sendsize = 0;
	do
	{
		int sendbyte = send(sock, &msg[sendsize], packet.ph.len - sendsize, 0);
		if (sendbyte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//비정상적인 종료
				return -1;
			}
		}
		sendsize += sendbyte;
	} while (sendsize < packet.ph.len);
	return sendsize;
}
int Server::SendMsg(NetUser* pUser, char* msg, int iSize, WORD type)
{
	// 보내는 패킷 풀에 저장하고 일괄 전송시스템.
	pUser->SendMsg(msg, iSize, type);
	return 0;
}
int Server::SendMsg(NetUser* pUser, UPACKET& packet)
{
	pUser->SendMsg(packet);
	return 0;
}
//한 유저가 입력한 메세지를 다른 유저에게 다 전송하는 함수
int Server::BroadcastUserPacketPool(NetUser* user)
{
	//유저의 패킷풀의 사이즈가 0보다 크면 전송할 패킷이 있다는 소리
	if (user->m_packetpool.size() > 0)
	{
		//iterator - 반복자
		list<Packet>::iterator iter;
		//패킷은 여러개로 쪼개져 전송되니 그 쪼개진 모든 패킷 패킷풀에 있는데 그거 첨부터 끝까지 훑기
		for (iter = user->m_packetpool.begin(); iter != user->m_packetpool.end();)
		{
			//패킷1 모든 유저한테 보내기 ->패킷2 모든 유저한테 보내기 -> ~ -> 마지막 패킷까지
			for (NetUser* senduser : m_UserList)
			{
				int i = SendMsg(senduser->m_sock, (*iter).m_upacket);
				//보낸게 0이면 유저 연결이 안되어 있다는것
				if (i <= 0)
				{
					senduser->m_connect = false;
				}
			}
			//보낸 패킷은 지우자
			iter = user->m_packetpool.erase(iter);
		}
	}
	return 1;
}int Server::Broadcast(Packet& t)
{
	for (NetUser* senduser : m_UserList)
	{
		int iRet = SendMsg(senduser->m_sock, t.m_upacket);
		if (iRet <= 0)
		{
			senduser->m_connect = false;
		}
	}
	return 1;
}