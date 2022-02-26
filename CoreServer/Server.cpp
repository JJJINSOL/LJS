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
				//���������� ����
				return -1;
			}
		}
		sendsize += sendbyte;
	} while (sendsize < packet.ph.len);
	return sendsize;
}
int Server::SendMsg(NetUser* pUser, char* msg, int iSize, WORD type)
{
	// ������ ��Ŷ Ǯ�� �����ϰ� �ϰ� ���۽ý���.
	pUser->SendMsg(msg, iSize, type);
	return 0;
}
int Server::SendMsg(NetUser* pUser, UPACKET& packet)
{
	pUser->SendMsg(packet);
	return 0;
}
//�� ������ �Է��� �޼����� �ٸ� �������� �� �����ϴ� �Լ�
int Server::BroadcastUserPacketPool(NetUser* user)
{
	//������ ��ŶǮ�� ����� 0���� ũ�� ������ ��Ŷ�� �ִٴ� �Ҹ�
	if (user->m_packetpool.size() > 0)
	{
		//iterator - �ݺ���
		list<Packet>::iterator iter;
		//��Ŷ�� �������� �ɰ��� ���۵Ǵ� �� �ɰ��� ��� ��Ŷ ��ŶǮ�� �ִµ� �װ� ÷���� ������ �ȱ�
		for (iter = user->m_packetpool.begin(); iter != user->m_packetpool.end();)
		{
			//��Ŷ1 ��� �������� ������ ->��Ŷ2 ��� �������� ������ -> ~ -> ������ ��Ŷ����
			for (NetUser* senduser : m_UserList)
			{
				int i = SendMsg(senduser->m_sock, (*iter).m_upacket);
				//������ 0�̸� ���� ������ �ȵǾ� �ִٴ°�
				if (i <= 0)
				{
					senduser->m_connect = false;
				}
			}
			//���� ��Ŷ�� ������
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