#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <list>
#include <WinSock2.h>
#include "Packet.h"
#include "NetUser.h"
list<NetUser> g_userlist;
//CRITICAL_SECTION - 쓰레드 동기화 개체(유저모드)
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
				//비정상적인 종료
				return -1;
			}
		}
		sendsize += sendbyte;
	} while (sendsize < packet.ph.len);
	return sendsize;
}
//한 유저가 입력한 메세지를 다른 유저에게 다 전송하는 함수
int Broadcast(NetUser& user)
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
		//뮤텍스에서는 waitfor - release 사이가 임계구역이 됨!
		WaitForSingleObject(g_Mutex, INFINITE);
		//DWORD dwID = GetCurrentThreadId();
		//std::cout << dwID << std::endl;
		
		//크리티컬섹션에서는 enter - leave 사이가 임계구역이 됨!
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

		//Sleep = 대기함수가 쓰레드들 스위칭 해줌!
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
	// 쓰레드 동기화 개체 (쓰레드끼리의 스위칭때문에 필요)
	// 크리티컬섹션(유저모드) / 뮤텍스(커널모드) - 이 두개가 비슷
	//InitializeCriticalSection - CriticalSection 초기 설정
	
	InitializeCriticalSection(&g_cs);
	
	//CreateMutex - Mutex 생성
	
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

	cout << "서버 가동중~" << endl;

	u_long on = 1;
	ioctlsocket(listensock, FIONBIO, &on);
	
	//RecvThread 생성===============================================================================
	DWORD Threadid;
	HANDLE hThreadRecv = ::CreateThread(0,0,RecvThread,(LPVOID)listensock,0, &Threadid);
	//CloseHandle - 종료한다는 것이 아닌, 내가 관여안하겠다는 의미
	CloseHandle(hThreadRecv);

	//SendThread 생성===============================================================================
	DWORD Threadidsend;
	HANDLE hThreadSend = ::CreateThread(0, 0, SendThread, (LPVOID)listensock, 0, &Threadidsend);
	//CloseHandle - 종료한다는 것이 아닌, 내가 관여안하겠다는 의미
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
			cout << g_userlist.size() << "명 접속중!" << endl;
		}
		Sleep(1);
	}
	closesocket(listensock);
	WSACleanup();
	DeleteCriticalSection(&g_cs);
	CloseHandle(g_Mutex);
}