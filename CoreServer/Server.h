#pragma once
#include "NetUser.h"
#include "Accepter.h"
struct XPacket
{
	NetUser* pUser;
	Packet   packet;
};
class Server : public ServerObj
{
	Accepter			m_Accept;
public:
	std::list<NetUser*> m_UserList;
	std::list<XPacket>	 m_packetPool;
	typedef std::list<NetUser*>::iterator m_UserIter;

	// #include <functional>
	using CallFunction = std::function<void(Packet& t, NetUser* user)>;
	typedef std::map<int, CallFunction>::iterator FuncionIterator;
	std::map<int, CallFunction> m_fnExecutePacket;

	virtual void LoginReq(Packet& t, NetUser* user);

	int SendMsg(SOCKET sock, UPACKET& packet);
	int SendMsg(NetUser* pUser, char* msg, int iSize, WORD type);
	int SendMsg(NetUser* pUser, UPACKET& packet);
	int Broadcast(Packet& t);
	int BroadcastUserPacketPool(NetUser* user);
	virtual bool AddUser(SOCKET socr, SOCKADDR_IN clientAddr);
protected:
	virtual bool DelUser(SOCKET sock);
	virtual bool DelUser(NetUser* pUser);
	virtual bool DelUser(m_UserIter& iter);
public:
	virtual bool	InitServer(int iPort);
	virtual bool	Run();
	virtual bool	Release();
};