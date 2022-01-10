#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <list>
#include <WinSock2.h>
#include "Packet.h"
#include "NetUser.h"
list<NetUser> g_userlist;
//CRITICAL_SECTION - ������ ����ȭ ��ü(�������)
CRITICAL_SECTION g_cs;
HANDLE g_Mutex;
#pragma comment (lib, "ws2_32.lib")
using namespace std;
//struct User
//{
//	SOCKET m_sock;
//	SOCKADDR_IN m_addr;
//	string m_name;
//	short m_port;
//	void set(SOCKET sock, SOCKADDR_IN addr)
//	{
//		m_sock = sock;
//		m_addr = addr;
//		m_name = inet_ntoa(addr.sin_addr);
//		m_port = ntohs(addr.sin_port);
//	}
//};
int SendMsg(SOCKET sock, char* msg, WORD type)
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
int SendMsg(SOCKET sock, UPACKET& packet)
{
	char* msg = (char*)&packet;
	int sendsize = 0;
	do
	{
		int sendbyte = send(sock, &msg[sendsize], packet.ph.len- sendsize, 0);
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
int Broadcast(NetUser& user)
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
int RecvUser(NetUser& user)
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
DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		//���ؽ������� waitfor - release ���̰� �Ӱ豸���� ��!
		WaitForSingleObject(g_Mutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		
		//ũ��Ƽ�ü��ǿ����� enter - leave ���̰� �Ӱ豸���� ��!
		//EnterCriticalSection(&g_cs);
		list<NetUser>::iterator iter;
		for (iter = g_userlist.begin(); iter != g_userlist.end();)
		{
			int ret = RecvUser(*iter);
			if (ret <= 0)
			{
				iter = g_userlist.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		ReleaseMutex(g_Mutex);
		//LeaveCriticalSection(&g_cs);

		//Sleep = ����Լ��� ������� ����Ī ����!
		Sleep(1);
	}
}
DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET socket = (SOCKET)param;
	while (1)
	{
		WaitForSingleObject(g_Mutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		//EnterCriticalSection(&g_cs);
		list<NetUser>::iterator iter;
		for (iter = g_userlist.begin(); iter != g_userlist.end();)
		{
			int ret = Broadcast(*iter);
			if (ret <= 0)
			{
				iter = g_userlist.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		ReleaseMutex(g_Mutex);
		//LeaveCriticalSection(&g_cs);
		Sleep(1);
	}
}
void main()
{
	//============================================================
	// ������ ����ȭ ��ü (�����峢���� ����Ī������ �ʿ�)
	// ũ��Ƽ�ü���(�������) / ���ؽ�(Ŀ�θ��) - �� �ΰ��� ���
	//InitializeCriticalSection - CriticalSection �ʱ� ����
	
	InitializeCriticalSection(&g_cs);
	
	//CreateMutex - Mutex ����
	
	g_Mutex = CreateMutex(NULL,FALSE,NULL);
	
	//============================================================
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET listensock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int a = bind(listensock, (sockaddr*)&sa, sizeof(sa));
	if (a == SOCKET_ERROR) return;
	a = listen(listensock, SOMAXCONN);
	if (a == SOCKET_ERROR) return;

	SOCKADDR_IN clientaddr;
	int len = sizeof(clientaddr);

	cout << "���� ������~" << endl;

	u_long on = 1;
	ioctlsocket(listensock, FIONBIO, &on);
	
	//RecvThread ����===============================================================================
	DWORD Threadid;
	HANDLE hThreadRecv = ::CreateThread(0,0,RecvThread,(LPVOID)listensock,0, &Threadid);
	//CloseHandle - �����Ѵٴ� ���� �ƴ�, ���� �������ϰڴٴ� �ǹ�
	CloseHandle(hThreadRecv);

	//SendThread ����===============================================================================
	DWORD Threadidsend;
	HANDLE hThreadSend = ::CreateThread(0, 0, SendThread, (LPVOID)listensock, 0, &Threadidsend);
	//CloseHandle - �����Ѵٴ� ���� �ƴ�, ���� �������ϰڴٴ� �ǹ�
	CloseHandle(hThreadSend);

	while (1)
	{
		SOCKET clientsock = accept(listensock, (sockaddr*)&clientaddr, &len);
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
			user.set(clientsock, clientaddr);

			EnterCriticalSection(&g_cs);
			g_userlist.push_back(user);
			LeaveCriticalSection(&g_cs);

			cout << "ip = " << inet_ntoa(clientaddr.sin_addr)
				<< " port = " << ntohs(clientaddr.sin_port)
				<< "  " << endl;
			u_long on = 1;
			ioctlsocket(clientsock, FIONBIO, &on);
			cout << g_userlist.size() << "�� ������!" << endl;
		}
		Sleep(1);
	}
	closesocket(listensock);
	WSACleanup();
	DeleteCriticalSection(&g_cs);
	CloseHandle(g_Mutex);
}