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
	//멀티캐스트는 서버,클라가 동일
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}

	SOCKET sockRecv = socket(AF_INET, SOCK_DGRAM, 0);
	//멀티캐스트틑 ip 변경으로 진행되기에 브로드캐스트 처럼 sock 변경이 필요없음
	//SOCKET sockSend = socket(AF_INET, SOCK_DGRAM, 0);
	int val = 1;
	char* opt = (char*)&val;
	int optlevel = IPPROTO_IP;
	int iption = IP_MULTICAST_LOOP;
	int optlen = sizeof(opt);
	//(소켓/옵션종류(프로토콜 레벨)/설정을 위한 소켓옵션번호/설정 값이 저장된 주소/버퍼 크기)
	//IPPROTO_IP - 멀티 캐스트 하려면 이 레벨 설정 / IPv4 / 멀티캐스트는 소켓변경X -> IP 변경 O 
	//IP_MULTICAST_LOOP - 멀티캐스트 소켓이 자신이 보낸 패킷을 수신하도록 함
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
	//===========================================멀티캐스트 그룹 가입========================================
	//ip_mreq - 구조체
	//멤버 변수1. imr_interface(멀티캐스트 패킷을 받을 네트워크 인터페이스) - INADDR_ANY 설정시 자동으로 설정
	//멤버 변수2. imr_multiaddr(가입/탈퇴할때 멀티캐스트 그룹 주소)
	ip_mreq mreq;
	//멀티캐스트 그룹 - 235.7.8.9
	mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	//멀티캐스트 그룹 가입 - 멀티캐스트 데이터를 받기 위해서는 반드시 멀티캐스트 그룹에 가입해야됨
	iRet = setsockopt(sockRecv, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	if (iRet == SOCKET_ERROR)
	{
		Error("error");
		return;
	}
	//=======================================================================================================
	// include 대상 포함.	- IP_ADD_SOURCE_MEMBERSHIP
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

	// exclude 대상 삭제========================================================================================
	//ip_mreq_source - 구조체
	//멤버 변수1. imr_sourceaddr(데이터를 수신할 주소)
	//멤버 변수2. imr_multiaddr(가입할 32비트 멀티캐스트 주소)
	//멤버 변수3. imr_interface(멀티캐스트 그룹에 가입할 로컬 인터페이스)
	ip_mreq_source mreqsource;
	mreqsource.imr_interface = mreq.imr_interface;
	mreqsource.imr_multiaddr = mreq.imr_multiaddr;
	//제어당할 주소
	mreqsource.imr_sourceaddr.s_addr = inet_addr("192.168.0.115");
	//IP_BLOCK_SOURCE - 대상 삭제
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

	//DROP 필수! - ip이기에
	//IP_DROP_MEMBERSHIP - 멀티캐스트 그룹 탈퇴
	setsockopt(sockRecv, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	WSACleanup();
}