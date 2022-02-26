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

	//WSAAsyncSelect - 윈도우 메세지 형태로 비동기적으로 소켓과 관련된 네트워크 이벤트 처리를 함
	//정해진 시간이 아니라 무작위로 메시지 큐에 메시지 날아옴
	//윈도우를 사용하니까 윈도우 핸들이 필요함
	//NETWORK_MSG - 윈도우가 받을 메시지
	//FD_CONNECT = 접속이 완료되면 윈도우 메시지 발생 등..
	if (WSAAsyncSelect(m_sock, hwnd,NETWORK_MSG,FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		return false;
	}
	//WSAConnect - 다른 소켓과 접속, 데이터 교환 등을 함
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
			strcpy_s(login.szID, "이진솔");
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