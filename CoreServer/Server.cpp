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

	cout << "서버 가동중~" << endl;

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
			cout << g_userlist.size() << "명 접속중!" << endl;
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
	//1. 패킷 생성
	UPACKET packet;
	//패킷 구조체 메모리 정리
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	//2. 패킷 전송
	//무엇을 보내고 받을때 직접 바로 보내는 것이 아니라, 운영체제 버퍼에 거쳐보냄
	//운영체제 sendbuffer/recvbuffer 꽉 차면 보내기 받기 안됨
	char* pmsg = (char*)&packet;
	int sendsize = 0;
	do
	{
		int sendbyte = send(sock, &pmsg[sendsize], packet.ph.len - sendsize, 0);
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
				//비정상적인 종료
				return -1;
			}
		}
		sendsize += sendbyte;
	} while (sendsize < packet.ph.len);
	return sendsize;
}
//한 유저가 입력한 메세지를 다른 유저에게 다 전송하는 함수
int Server:: Broadcast(NetUser& user)
{
	//유저의 패킷풀의 사이즈가 0보다 크면 전송할 패킷이 있다는 소리
	if (user.m_packetpool.size() > 0)
	{
		//iterator - 반복자
		list<Packet>::iterator iter;
		//패킷은 여러개로 쪼개져 전송되니 그 쪼개진 모든 패킷 패킷풀에 있는데 그거 첨부터 끝까지 훑기
		for (iter = user.m_packetpool.begin(); iter != user.m_packetpool.end();)
		{
			//패킷1 모든 유저한테 보내기 ->패킷2 모든 유저한테 보내기 -> ~ -> 마지막 패킷까지
			for (NetUser& senduser : g_userlist)
			{
				int i = SendMsg(senduser.m_sock, (*iter).m_upacket);
				//보낸게 0이면 유저 연결이 안되어 있다는것
				if (i <= 0)
				{
					senduser.m_connect = false;
				}
			}
			//보낸 패킷은 지우자
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