#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include "ServerStd.h"
#include "ServerObj.h"
#include "Packet.h"
//#pragma comment (lib, "ws2_32.lib")
using namespace std;

//OVERLAPPED - Ȯ����
struct OV
{
	OVERLAPPED ov;
	//Ű�� ����
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

	//����
	OV m_ovRecv;
	OV m_ovSend;
	//WSABUF - ����ü/���ۿ� ���� ũ��
	WSABUF m_wsaRecvBuffer;
	WSABUF m_wsaSendBuffer;
	char m_szRecv[256];
	char m_szSend[256];

	char m_recvbuffer[2048];
	int  m_packetpos; // ��Ŷ�� �����ּ�
	int  m_writepos; // ������ ���� ��ġ
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

