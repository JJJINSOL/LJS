#include "NetWork.h"
list<NetUser> userlist;
int SendMsg(SOCKET sock, char* msg, WORD type)
{
	//1. 패킷 생성
	UPACKET packet;
	//패킷 구조체 메모리 정리
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	//2. 패킷 전송
	//무엇을 보내고 받을때 직접 바로 보내는 것이 아니라, 운영체제 버퍼에 거쳐보냄
	//운영체제 sendbuffer/recvbuffer 꽉 차면 보내기 받기 안됨
	char* pmsg = (char*)&packet;
	int sendsize = 0;
	do
	{
		int sendbyte = send(sock, &pmsg[sendsize], packet.ph.len - sendsize, 0);
		if (sendbyte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//비정상적인 종료
				return -1;
			}
		}
		sendsize += sendbyte;
	} while (sendsize < packet.ph.len);
	return sendsize;
}
int SendMsg(SOCKET sock, UPACKET& packet)
{
	char* msg = (char*)&packet;
	int sendsize = 0;
	do
	{
		int sendbyte = send(sock, &msg[sendsize], packet.ph.len- sendsize, 0);
		if (sendbyte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//비정상적인 종료
				return -1;
			}
		}
		sendsize += sendbyte;
	} while (sendsize < packet.ph.len);
	return sendsize;
}
int AddUser(SOCKET sock)
{
	SOCKADDR_IN clientaddr;
	int len = sizeof(clientaddr);
	SOCKET clientsock = accept(sock, (sockaddr*)&clientaddr, &len);
	if (clientsock == SOCKET_ERROR)
	{
		return -1;
	}
	else
	{
		NetUser user;
		user.set(clientsock, clientaddr);
		userlist.push_back(user);

		cout << "ip = " << inet_ntoa(clientaddr.sin_addr)
			<< " port = " << ntohs(clientaddr.sin_port)
			<< "  " << endl;

		cout << userlist.size() << "명 접속중!" << endl;
	}
	return 1;
}
int RecvUser(NetUser& user)
{
	char recvbuffer[1024] = { 0, };
	int recvbyte = recv(user.m_sock, recvbuffer, 1024, 0);
	if (recvbyte == 0)
	{
		return 0;
	}
	if (recvbyte == SOCKET_ERROR)
	{
		return -1;
	}
	user.DispatchRead(recvbuffer, recvbyte);
	return 1;
}
void main()
{
	NetWork nw;
	nw.Initnetwork();
	nw.Initserver(SOCK_STREAM, 10000,nullptr);
	cout << "서버 가동중~" << endl;
	//select를 사용하면 넌블록킹이 아니어도 됨

	FD_SET rSet; // 읽기 셋
	FD_SET wSet; // 쓰기 셋

	//select 함수가 블로킹되는 것을 피하기 위해 타임아웃 설정
	timeval timeout; // 타임아웃 값
	timeout.tv_sec = 0; // 초
	timeout.tv_usec = 1; // 마이크로 초

	while (1)
	{
		//1.소켓 셋 비운다(초기화)
		//2.소켓 셋에 소켓을 넣음. 셋에 넣을 수 있는 최대 개수 = 64
		//3.select() 함수 호출
		//4.select() 함수 리턴 후, 소켓 셋에 남아 있는 모든 소켓에 대해 적절한 소켓 함수를 호출 처리
		//1-4 반복 

		//1번 FD_ZERO =  초기화
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);

		//2번 //디스크립터 설정 - 변화를 확인할 파일 디스크립터들을 한 묶음으로 모아둔다
		//FD_SET = 파일 디스크립터를 모아두는 비트단위 자료형
		FD_SET(nw.m_listensock, &rSet);

		list<NetUser>::iterator userIter;

		for (userIter = userlist.begin(); userIter != userlist.end();)
		{
			if ((*userIter).m_connect == false)
			{
				cout << (*userIter).m_name << " 접속 종료되었음" << endl;
				userIter = userlist.erase(userIter);
				continue;
			}
			FD_SET((*userIter).m_sock, &rSet);
			//m_packetpool - 유저의 정보가 있는 패킷
			if ((*userIter).m_packetpool.size() > 0)
			{
				FD_SET((*userIter).m_sock, &wSet);
			}
			userIter++;
		}
		//3번 - 여러 파일 디스크립터를 검사하고, 그 결과 전달
		// -1 = 오류 / 0 = 타임아웃 / 0보다 큰수 = 변화발생 파일 디스크립터 수
		//readfds = &rSet - 이 리스트에 있는 식별자들은 시스템에 의해 즉시 입력이 가능한지 확인된다. 즉, 입력 가능으로 인해 반환된 식별자에 대해 recv()는 블로킹되지 않는다.
		//writefds = &wSet - 이 리스트에 있는 식별자들은 시스템에 의해 즉시 출력이 가능한지 확인된다. 즉, 출력 가능으로 인해 반환된 식별자에 대해 send()는 블로킹되지 않는다.
		//excepfds = NULL - 이 리스트에 있는 식별자들은 시스템에 의해 예상되는 예외 사항이나 에러가 발생했는지 확인된다.
		int ret = select(0, &rSet, &wSet, NULL, &timeout);
		if (ret == 0)
		{
			continue;
		}
		if (FD_ISSET(nw.m_listensock, &rSet))
		{
			if (AddUser(nw.m_listensock) <= 0)
			{
				break;
			}
		}
		for (NetUser& user : userlist)
		{
			if (FD_ISSET(user.m_sock, &rSet))
			{
				if (RecvUser(user) <= 0)
				{
					user.m_connect = false;
				}
			}
		}
		for (NetUser& user : userlist)
		{
			if (FD_ISSET(user.m_sock, &wSet))
			{
				if (user.m_packetpool.size() > 0)
				{
					list<Packet>::iterator iter;
					for (iter = user.m_packetpool.begin(); iter != user.m_packetpool.end();)
					{
						for (NetUser& senduser : userlist)
						{
							int ret = SendMsg(senduser.m_sock, (*iter).m_upacket);
							if (ret <= 0)
							{
								senduser.m_connect = false;
							}
						}
						iter = user.m_packetpool.erase(iter);
					}
				}
			}
		}
				
	}
		nw.Closenetwork();
}