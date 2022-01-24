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
	//INADDR_BROADCAST - 연결된 모든 유저에게!
	addressSend.sin_addr.s_addr = INADDR_BROADCAST;

	{
		while (1)
		{
			fgets(SendBuffer, 256, stdin);
			//sendto - UDP에서 데이터 보내기
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
	//보낼때는 소켓 설정없이 생성만 하면 된당! - 포트번호 필요없음(어차피 보낼때 포트번호 달라짐)
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

	//얘는 받는 소켓이니 밑에 설정 필요!
	SOCKET sockRecv = socket(AF_INET, SOCK_DGRAM, 0);
	char flag = true;
	//setsockopt - 소켓 옵션 설정하기
	//(소켓/옵션종류(프로토콜 레벨)/설정을 위한 소켓옵션번호/설정 값이 저장된 주소/버퍼 크기)
	//SOL_SOCKET - 가장 일반적인 옵션(레벨) - 이것으로 해야지 브로드캐스트 옵션 설정 가능
	//SO_BROADCAST - 브로드캐시트 메시지 전달
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0)
	{
		closesocket(sock);
	}

	//받을때만 필요 (sockRecv)========================================
	int iRet = -1;
	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9000);
	address.sin_addr.s_addr = INADDR_ANY; //모든 ip
	//UDP는 UDP당 1개의 ip이므로 bind 한번, 하지만 혼자 디버그할때는 이 프로젝트로 하면 ip여러개 되므로 recv/send 프젝으로 하자.
	iRet = bind(sockRecv, (SOCKADDR*)&address, sizeof(address));
	//================================================================

	unsigned int id;
	unsigned long hSendThread = _beginthreadex(NULL, 0, SendThread, (LPVOID)sock, 0, &id);

	SOCKADDR_IN recvAddr;
	char RecvBuf[256] = { 0, };
	INT addlen = sizeof(recvAddr);
	while (1)
	{
		//recvfrom - UDP에서 데이터 받기
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