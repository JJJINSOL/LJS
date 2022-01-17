#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include "ServerStd.h"
#include "ServerObj.h"
#include "Packet.h"
//#pragma comment (lib, "ws2_32.lib")
using namespace std;

//OVERLAPPED - 확장형
struct OV
{
	OVERLAPPED ov;
	//키값 구분
	int  type;
};
class NetUser : public ServerObj
{
public:
	SOCKET m_sock;
	SOCKADDR_IN m_addr;
	string m_name;
	short m_port;
	bool m_connect = false;

	//버퍼
	OV m_ovRecv;
	OV m_ovSend;
	//WSABUF - 구조체/버퍼와 버퍼 크기
	WSABUF m_wsaRecvBuffer;
	WSABUF m_wsaSendBuffer;
	char m_szRecv[256];
	char m_szSend[256];

	char m_recvbuffer[2048];
	int  m_packetpos; // 패킷의 시작주소
	int  m_writepos; // 현재의 저장 위치
	int  m_readpos;

	list<Packet> m_packetpool;
	void set(SOCKET sock, SOCKADDR_IN addr);

	int Dispatch(DWORD dwTrans, OV* tov);
	int DispatchRecv(char* recvbuffer, int recvbyte);
	int DispatchSend(DWORD dwTrans);

	int  Recv();
	bool DisConnect();

	NetUser();
	virtual ~NetUser();
};

