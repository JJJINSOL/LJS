#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include "ServerStd.h"
#include "ServerObj.h"
#include "Packet.h"
#include "ObjectPool.h"
//#pragma comment (lib, "ws2_32.lib")
using namespace std;
class Server;
//OVERLAPPED - 확장형
// 비동기 작업이 완료 시점까지 OVERLAPPED 유지되어 있어야 된다.
struct OV : public ObjectPool<OV>
{
	enum { MODE_RECV = 1, MODE_SEND = 2, MODE_EXIT };
	OVERLAPPED ov;
	int  type;
	OV(int packetType)
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		type = packetType;
	}
	OV()
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		type = MODE_RECV;
	}
};
class NetUser : public ServerObj
{
	Server* m_pServer = nullptr;
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
	char m_szRecv[256];//256
	char m_szSend[256];

	char m_recvbuffer[2048];
	int  m_packetpos; // 패킷의 시작주소
	int  m_writepos; // 현재의 저장 위치
	int  m_readpos;

	list<Packet> m_packetpool;
	void set(SOCKET sock, SOCKADDR_IN addr, Server* pServer);

	int Dispatch(DWORD dwTrans, OV* tov);
	int DispatchRecv(char* recvbuffer, int recvbyte);
	int DispatchSend(DWORD dwTrans);

	int  SendMsg(char* msg, int iSize, WORD type);
	int  SendMsg(UPACKET& packet);

	int  Recv();
	bool DisConnect();

	NetUser();
	virtual ~NetUser();
};

