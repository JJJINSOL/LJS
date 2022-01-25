#include "Sample.h"
LRESULT  Sample::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		//WM_COMMAND = 명령 있으면 ex. 확인버튼 입력
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
		case 200:
		{
			char buffer[MAX_PATH] = { 0, };
			SendMessageA(m_edit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
			Packet packet(PACKET_CHAT_MSG);
			packet << 970214 << "이진솔"<< buffer;
			m_msg.SendMsg(m_net.m_sock, packet.m_upacket);

			SendMessageA(m_edit, WM_SETTEXT, 0, (LPARAM)"");
		}break;
		}
	}break;
	}
	return m_net.MsgProc(hwnd, msg, wparam, lparam);
}
bool Sample::Init()
{
	//윈도우창 스타일
	DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
	m_edit = CreateWindow(L"edit", NULL, style, 0, g_rtClient.bottom - 50, 250, 50, m_hWnd, (HMENU)100, m_hInsatance, NULL);
	style = WS_CHILD | WS_VISIBLE;
	m_button = CreateWindow(L"button", L"전송", style, 250, g_rtClient.bottom - 50, 50, 50, m_hWnd, (HMENU)200, m_hInsatance, NULL);
	m_listbox = CreateWindow(L"listbox", NULL, style, 0, 0, 300, g_rtClient.bottom - 70, m_hWnd, (HMENU)300, m_hInsatance, NULL);

	SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)"채팅서버에 입장하셨습니다!");

	m_net.Initnetwork();
	//"192.168.0.12"
	//"192.168.0.90"
	//"27.35.45.57"
	m_net.Connect(g_hwnd, SOCK_STREAM, 10000, "49.142.62.157");
	return true;
}
bool Sample::Frame()
{
	int count = m_net.m_user.m_packetPool.size();
	if (count > 0 && m_chatcount != count)
	{
		m_chatcount = count;

		SendMessage(m_listbox, LB_RESETCONTENT, 0, 0);
		list<Packet> ::iterator iter;
		if (m_net.m_user.m_packetPool.size() > 20)
		{
			m_net.m_user.m_packetPool.pop_front();
		}
		for (iter = m_net.m_user.m_packetPool.begin(); iter != m_net.m_user.m_packetPool.end(); iter++)
		{
			ChatMsg recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			(*iter) >> recvdata.index >> recvdata.name >> recvdata.message;
			//SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)recvdata.name);
			SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)recvdata.message);
			//iter = m_Net.m_PlayerUser.m_packetPool.erase(iter);
			(*iter).Reset();
		}
	}
	return true;
}
bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{
	m_net.Closenetwork();
	return true;
}
Sample::Sample()
{

}
Sample:: ~Sample()
{

}
RUN();