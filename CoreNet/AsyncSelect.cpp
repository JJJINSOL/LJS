#include "AsyncSelect.h"
bool AsyncSelect:: Connect(HWND hwnd, int protocol, int port, const char* ip)
{
	m_sock = socket(AF_INET, protocol, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);
	m_playuser.m_sock = m_sock;

	//WSAAsyncSelect - ������ �޼��� ���·� �񵿱������� ���ϰ� ���õ� ��Ʈ��ũ �̺�Ʈ ó���� ��
	//������ �ð��� �ƴ϶� �������� �޽��� ť�� �޽��� ���ƿ�
	//�����츦 ����ϴϱ� ������ �ڵ��� �ʿ���
	//NETWORK_MSG - �����찡 ���� �޽���
	//FD_CONNECT = ������ �Ϸ�Ǹ� ������ �޽��� �߻� ��..
	if (WSAAsyncSelect(m_sock, hwnd,NETWORK_MSG,FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		return false;
	}
	//WSAConnect - �ٸ� ���ϰ� ����, ������ ��ȯ ���� ��
	int ret = WSAConnect(m_sock, (sockaddr*)&sa,sizeof(sa),NULL, NULL, NULL, NULL);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
LRESULT AsyncSelect:: MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case NETWORK_MSG:
	{
		WORD ret = WSAGETSELECTEVENT(lparam);
		switch (ret)
		{
		case FD_CONNECT:
		{
			LoginReq login;
			strcpy_s(login.szID, "������");
			strcpy_s(login.szPS, "1111");
			SendMsg(m_sock, (char*)&login, sizeof(LoginReq), PACKET_LOGIN_REQ);

			m_connect = true;
		}break;
		case FD_READ:
		{
			RecvUser(m_playuser);
		}break;
		case FD_WRITE:
		{
			m_connect = false;
		}break;
		case FD_CLOSE:
		{
			m_connect = false;
		}break;
		}
	}break;
	}
	return 0;
}