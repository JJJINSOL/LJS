#include "Server.h"

bool Server:: InitServer(int port)
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	//���� ����
	m_socket = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	//���Ͽ� �ּ��Ҵ�
	int ret = bind(m_socket,(sockaddr*)&sa,sizeof(sa));
	if (ret == SOCKET_ERROR)return false;
	//���� ��û�� ����ϴ� �Լ�
	ret = listen(m_socket, SOMAXCONN);
	if (ret == SOCKET_ERROR)return false;
	cout << "���� ������!" << endl;

	//non-blocking ����
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