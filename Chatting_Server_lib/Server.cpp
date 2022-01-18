#include "Server.h"

bool Server:: InitServer(int port)
{
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

	//non-blocking 소켓
	u_long on = 1;
	ioctlsocket(m_socket, FIONBIO, &on);

	return true;
}

Server:: Server()
{

}
Server:: ~Server()
{

}