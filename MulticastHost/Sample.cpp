#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")
void Error(const char* msg)
{
	LPVOID lpMsgBuffer;
	int iError = WSAGetLastError();
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, iError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuffer, 0, NULL);
	printf("%s", lpMsgBuffer);
	LocalFree(lpMsgBuffer);
	//exit(-1);
}
unsigned WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char SendBuffer[256] = { 0, };

	SOCKADDR_IN multicastAddress;
	ZeroMemory(&multicastAddress, sizeof(multicastAddress));
	multicastAddress.sin_family = AF_INET;
	multicastAddress.sin_port = htons(9000);
	multicastAddress.sin_addr.s_addr = inet_addr("235.7.8.9");
	{
		while (1)
		{
			fgets(SendBuffer, 256, stdin);
			int iRet = sendto(sock, SendBuffer, strlen(SendBuffer), 0, (SOCKADDR*)&multicastAddress, sizeof(multicastAddress));
			if (iRet == SOCKET_ERROR)
			{
				break;
			}
		}
	}
	return 0;
}
void main()
{
	//��Ƽĳ��Ʈ�� ����,Ŭ�� ����
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}

	SOCKET sockRecv = socket(AF_INET, SOCK_DGRAM, 0);
	//��Ƽĳ��Ʈ�z ip �������� ����Ǳ⿡ ��ε�ĳ��Ʈ ó�� sock ������ �ʿ����
	//SOCKET sockSend = socket(AF_INET, SOCK_DGRAM, 0);
	int val = 1;
	char* opt = (char*)&val;
	int optlevel = IPPROTO_IP;
	int iption = IP_MULTICAST_LOOP;
	int optlen = sizeof(opt);
	//(����/�ɼ�����(�������� ����)/������ ���� ���Ͽɼǹ�ȣ/���� ���� ����� �ּ�/���� ũ��)
	//IPPROTO_IP - ��Ƽ ĳ��Ʈ �Ϸ��� �� ���� ���� / IPv4 / ��Ƽĳ��Ʈ�� ���Ϻ���X -> IP ���� O 
	//IP_MULTICAST_LOOP - ��Ƽĳ��Ʈ ������ �ڽ��� ���� ��Ŷ�� �����ϵ��� ��
	int iRet = setsockopt(sockRecv, IPPROTO_IP, IP_MULTICAST_LOOP, opt, sizeof(opt));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = INADDR_ANY;// inet_addr("192.168.0.12");
	iRet = bind(sockRecv, (SOCKADDR*)&address, sizeof(address));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	//===========================================��Ƽĳ��Ʈ �׷� ����========================================
	//ip_mreq - ����ü
	//��� ����1. imr_interface(��Ƽĳ��Ʈ ��Ŷ�� ���� ��Ʈ��ũ �������̽�) - INADDR_ANY ������ �ڵ����� ����
	//��� ����2. imr_multiaddr(����/Ż���Ҷ� ��Ƽĳ��Ʈ �׷� �ּ�)
	ip_mreq mreq;
	//��Ƽĳ��Ʈ �׷� - 235.7.8.9
	mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	//��Ƽĳ��Ʈ �׷� ���� - ��Ƽĳ��Ʈ �����͸� �ޱ� ���ؼ��� �ݵ�� ��Ƽĳ��Ʈ �׷쿡 �����ؾߵ�
	iRet = setsockopt(sockRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	if (iRet == SOCKET_ERROR)
	{
		Error("error");
		return;
	}
	//=======================================================================================================
	// include ��� ����.	- IP_ADD_SOURCE_MEMBERSHIP
	/*ip_mreq_source mreqsource;
	mreqsource.imr_interface = mreq.imr_interface;
	mreqsource.imr_multiaddr = mreq.imr_multiaddr;
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.40");
	setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.41");
	setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.42");
	setsockopt(sockRecv, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));*/

	// exclude ��� ����========================================================================================
	//ip_mreq_source - ����ü
	//��� ����1. imr_sourceaddr(�����͸� ������ �ּ�)
	//��� ����2. imr_multiaddr(������ 32��Ʈ ��Ƽĳ��Ʈ �ּ�)
	//��� ����3. imr_interface(��Ƽĳ��Ʈ �׷쿡 ������ ���� �������̽�)
	ip_mreq_source mreqsource;
	mreqsource.imr_interface = mreq.imr_interface;
	mreqsource.imr_multiaddr = mreq.imr_multiaddr;
	//������� �ּ�
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.115");
	//IP_BLOCK_SOURCE - ��� ����
	iRet = setsockopt(sockRecv, IPPROTO_IP, IP_BLOCK_SOURCE,(char*)&mreqsource, sizeof(mreqsource));
	if (iRet == SOCKET_ERROR)
	{
		Error("error");
		return;
	}
	//===========================================================================================================

	unsigned int id;
	unsigned long hSendThread = _beginthreadex(NULL, 0, SendThread, (LPVOID)sockRecv, 0, &id);

	SOCKADDR_IN recvAddr;
	char buf[256] = { 0, };
	INT addlen = sizeof(recvAddr);

	while (1)
	{
		iRet = recvfrom(sockRecv, buf, 256, 0, (SOCKADDR*)&recvAddr, &addlen);
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
		buf[iRet] = 0;
		printf("\n[%s:%d]:%s", inet_ntoa(recvAddr.sin_addr), ntohs(recvAddr.sin_port), buf);
	}

	//DROP �ʼ�! - ip�̱⿡
	//IP_DROP_MEMBERSHIP - ��Ƽĳ��Ʈ �׷� Ż��
	setsockopt(sockRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	WSACleanup();
}