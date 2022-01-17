#include "Server.h"
bool Server::InitServer(int port)
{
	InitializeCriticalSection(&g_cs);

	g_Mutex = CreateMutex(NULL, FALSE, NULL);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	m_listensock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int a = bind(m_listensock, (sockaddr*)&sa, sizeof(sa));
	if (a == SOCKET_ERROR) return false;
	a = listen(m_listensock, SOMAXCONN);
	if (a == SOCKET_ERROR) return false;

	//SOCKADDR_IN clientaddr;
	//int len = sizeof(clientaddr);

	cout << "���� ������~" << endl;

	u_long on = 1;
	ioctlsocket(m_listensock, FIONBIO, &on);
	return true;
}
bool Server:: Run()
{
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	while (1)
	{
		SOCKET clientsock = accept(m_listensock, (sockaddr*)&clientAddr, &len);
		if (clientsock == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				cout << "ERRORCODE = " << error << endl;
				break;
			}
		}
		else
		{
			NetUser user;
			user.set(clientsock, clientAddr);

			//EnterCriticalSection(&g_cs);
			WaitForSingleObject(g_Mutex, INFINITE);
			g_userlist.push_back(user);
			//LeaveCriticalSection(&g_cs);
			ReleaseMutex(g_Mutex);

			cout << "ip = " << inet_ntoa(clientAddr.sin_addr)
				<< " port = " << ntohs(clientAddr.sin_port)
				<< "  " << endl;
			u_long on = 1;
			ioctlsocket(clientsock, FIONBIO, &on);
			cout << g_userlist.size() << "�� ������!" << endl;
		}
		Sleep(1);
	}
	return true;
}
bool Server:: Release()
{
	closesocket(m_listensock);
	WSACleanup();
	DeleteCriticalSection(&g_cs);
	CloseHandle(g_Mutex);
	return true;
}

int Server:: SendMsg(SOCKET sock, char* msg, WORD type)
{
	//1. ��Ŷ ����
	UPACKET packet;
	//��Ŷ ����ü �޸� ����
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	//2. ��Ŷ ����
	//������ ������ ������ ���� �ٷ� ������ ���� �ƴ϶�, �ü�� ���ۿ� ���ĺ���
	//�ü�� sendbuffer/recvbuffer �� ���� ������ �ޱ� �ȵ�
	char* pmsg = (char*)&packet;
	int sendsize = 0;
	do
	{
		int sendbyte = send(sock, &pmsg[sendsize], packet.ph.len - sendsize, 0);
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
int Server:: SendMsg(SOCKET sock, UPACKET& packet)
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
//�� ������ �Է��� �޼����� �ٸ� �������� �� �����ϴ� �Լ�
int Server:: Broadcast(NetUser& user)
{
	//������ ��ŶǮ�� ����� 0���� ũ�� ������ ��Ŷ�� �ִٴ� �Ҹ�
	if (user.m_packetpool.size() > 0)
	{
		//iterator - �ݺ���
		list<Packet>::iterator iter;
		//��Ŷ�� �������� �ɰ��� ���۵Ǵ� �� �ɰ��� ��� ��Ŷ ��ŶǮ�� �ִµ� �װ� ÷���� ������ �ȱ�
		for (iter = user.m_packetpool.begin(); iter != user.m_packetpool.end();)
		{
			//��Ŷ1 ��� �������� ������ ->��Ŷ2 ��� �������� ������ -> ~ -> ������ ��Ŷ����
			for (NetUser& senduser : g_userlist)
			{
				int i = SendMsg(senduser.m_sock, (*iter).m_upacket);
				//������ 0�̸� ���� ������ �ȵǾ� �ִٴ°�
				if (i <= 0)
				{
					senduser.m_connect = false;
				}
			}
			//���� ��Ŷ�� ������
			iter = user.m_packetpool.erase(iter);
		}
	}
	return 1;
}
int Server:: RecvUser(NetUser& user)
{
	char recvbuffer[1024] = { 0, };
	int recvbyte = recv(user.m_sock, recvbuffer, 1024, 0);
	if (recvbyte == 0)
	{
		return 0;
	}
	if (recvbyte == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 2;
	}
	user.DispatchRead(recvbuffer, recvbyte);
	return 1;
}