#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

unsigned WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char SendBuffer[256] = { 0, };

	SOCKADDR_IN addressSend;
	ZeroMemory(&addressSend, sizeof(addressSend));
	addressSend.sin_family = AF_INET;
	addressSend.sin_port = htons(9000);
	//INADDR_BROADCAST - ����� ��� ��������!
	addressSend.sin_addr.s_addr = INADDR_BROADCAST;

	{
		while (1)
		{
			fgets(SendBuffer, 256, stdin);
			//sendto - UDP���� ������ ������
			int iRet = sendto(sock, SendBuffer, strlen(SendBuffer), 0, (SOCKADDR*)&addressSend, sizeof(addressSend));
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
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}
	//SOCK_DGRAM - UDP
	//�������� ���� �������� ������ �ϸ� �ȴ�! - ��Ʈ��ȣ �ʿ����(������ ������ ��Ʈ��ȣ �޶���)
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

	//��� �޴� �����̴� �ؿ� ���� �ʿ�!
	SOCKET sockRecv = socket(AF_INET, SOCK_DGRAM, 0);
	char flag = true;
	//setsockopt - ���� �ɼ� �����ϱ�
	//(����/�ɼ�����(�������� ����)/������ ���� ���Ͽɼǹ�ȣ/���� ���� ����� �ּ�/���� ũ��)
	//SOL_SOCKET - ���� �Ϲ����� �ɼ�(����) - �̰����� �ؾ��� ��ε�ĳ��Ʈ �ɼ� ���� ����
	//SO_BROADCAST - ��ε�ĳ��Ʈ �޽��� ����
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0)
	{
		closesocket(sock);
	}

	//�������� �ʿ� (sockRecv)========================================
	int iRet = -1;
	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = INADDR_ANY; //��� ip
	//UDP�� UDP�� 1���� ip�̹Ƿ� bind �ѹ�, ������ ȥ�� ������Ҷ��� �� ������Ʈ�� �ϸ� ip������ �ǹǷ� recv/send �������� ����.
	iRet = bind(sockRecv, (SOCKADDR*)&address, sizeof(address));
	//================================================================

	unsigned int id;
	unsigned long hSendThread = _beginthreadex(NULL, 0, SendThread, (LPVOID)sock, 0, &id);

	SOCKADDR_IN recvAddr;
	char RecvBuf[256] = { 0, };
	INT addlen = sizeof(recvAddr);
	while (1)
	{
		//recvfrom - UDP���� ������ �ޱ�
		iRet = recvfrom(sockRecv, RecvBuf, 256, 0, (SOCKADDR*)&recvAddr, &addlen);
		if (iRet == SOCKET_ERROR)
		{
			break;
		}
		RecvBuf[iRet] = 0;
		printf("\n[%s:%d]:%s", inet_ntoa(recvAddr.sin_addr), ntohs(recvAddr.sin_port), RecvBuf);
	}
	WSACleanup();
}