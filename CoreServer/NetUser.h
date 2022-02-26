#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include "ServerStd.h"
#include "ServerObj.h"
#include "Packet.h"
#include "ObjectPool.h"
//#pragma comment (lib, "ws2_32.lib")
using namespace std;
class Server;
//OVERLAPPED - Ȯ����
// �񵿱� �۾��� �Ϸ� �������� OVERLAPPED �����Ǿ� �־�� �ȴ�.
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

	//����
	OV m_ovRecv;
	OV m_ovSend;
	//WSABUF - ����ü/���ۿ� ���� ũ��
	WSABUF m_wsaRecvBuffer;
	WSABUF m_wsaSendBuffer;
	char m_szRecv[256];//256
	char m_szSend[256];

	char m_recvbuffer[2048];
	int  m_packetpos; // ��Ŷ�� �����ּ�
	int  m_writepos; // ������ ���� ��ġ
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

