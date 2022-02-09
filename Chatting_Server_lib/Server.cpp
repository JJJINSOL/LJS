#include "Server.h"
bool Server:: InitServer(int port)
{
	//InitializeCriticalSection(&m_cs);
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	//소켓 생성
	m_socket = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	//소켓에 주소할당
	int ret = bind(m_socket,(sockaddr*)&sa,sizeof(sa));
	if (ret == SOCKET_ERROR)return false;
	//연결 요청을 대기하는 함수
	ret = listen(m_socket, SOMAXCONN);
	if (ret == SOCKET_ERROR)return false;
	cout << "서버 가동중!" << endl;
	cout << m_userlist.size() << "명 접속중입니다." << endl;

	//non-blocking 소켓
	u_long on = 1;
	ioctlsocket(m_socket, FIONBIO, &on);

	//m_server = this;

	return true;
}
bool Server:: Run()
{
	return  true;
}
bool Server:: Release()
{
	closesocket(m_socket);
	WSACleanup();
	//DeleteCriticalSection(&m_cs);
	return true;
}
bool Server:: AddUser(SOCKET sock, SOCKADDR_IN clientaddr)
{
	return true;
}
//한 유저가 입력한 메세지를 다른 유저에게 다 전송하는 함수
bool Server::Broadcast(User* user)
{
	//유저의 패킷풀의 사이즈가 0보다 크면 전송할 패킷이 있다는 소리
	if (user->m_packetPool.size() > 0)
	{
		//iterator - 반복자
		list<Packet>::iterator iter;
		//패킷은 여러개로 쪼개져 전송되니 그 쪼개진 모든 패킷 패킷풀에 있는데 그거 첨부터 끝까지 훑기
		for (iter = user->m_packetPool.begin(); iter != user->m_packetPool.end();)
		{
			list<User*>::iterator suser;
			//패킷1 모든 유저한테 보내기 ->패킷2 모든 유저한테 보내기 -> ~ -> 마지막 패킷까지
			for (suser = m_userlist.begin(); suser != m_userlist.end();)
			{
				User* senduser = (User*)*suser;
				Message m;
				int i = m.SendMsg(senduser->m_sock, (*iter).m_upacket);
				//보낸게 0이면 유저 연결이 안되어 있다는것
				if (i <= 0)
				{
					senduser->m_connect = false;
					suser = m_userlist.erase(suser);
				}
				suser++;
			}
			//보낸 패킷은 지우자
			iter = user->m_packetPool.erase(iter);
		}
	}
	return true;
}
Server:: Server()
{
	
}
Server:: ~Server()
{

}