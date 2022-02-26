#include "LobbyServer.h"
void LobbyServer::LoginReq(Packet& t, NetUser* user)
{
	TLoginReq login;
	memcpy(&login, t.m_upacket.msg, sizeof(TLoginReq));
	// DB 승인받고 반환
	LoginAck ack;
	ack.iResult = 1;
	SendMsg(user,
		(char*)&ack,
		sizeof(LoginAck),
		PACKET_LOGIN_ACK);
}
void LobbyServer::ChatMsg(Packet& t, NetUser* user)
{
	Broadcast(t);
}
bool LobbyServer::Run()
{
	while (1)
	{
		EnterCriticalSection(&m_cs);
		//for (TNetUser* tUser : m_UserList)
		//{
		//	TChatUser* pChat = (TChatUser*)tUser;
		//	if (pChat->m_packetPool.size() > 0)
		//	{
		//		// tUser->Process();				
		//	}
		//}
		// 패킷처리
		std::list<XPacket>::iterator iter;
		for (iter = m_packetPool.begin();
			iter != m_packetPool.end();
			iter++)
		{
			XPacket* xp = (XPacket*)&(*iter);
			FuncionIterator iter =
				m_fnExecutePacket.find(xp->packet.m_upacket.ph.type);
			if (iter != m_fnExecutePacket.end())
			{
				CallFunction call = iter->second;
				call(xp->packet, xp->pUser);
			}
		}
		m_packetPool.clear();

		// 주기적인 동기화
		/*for (TNetUser* tUser : m_UserList)
		{
			TChatUser* pChat = (TChatUser*)tUser;
			if (pChat->m_packetPool.size() > 0)
			{
				Broadcast(tUser);
			}
		}*/


		for (m_UserIter iter = m_UserList.begin();
			iter != m_UserList.end();)
		{
			if ((*iter)->m_connect == false)
			{
				DelUser((*iter));
				delete (*iter);
				iter = m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		LeaveCriticalSection(&m_cs);
		Sleep(1);
	}
	return true;
}