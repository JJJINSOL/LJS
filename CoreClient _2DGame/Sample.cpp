#include "Sample.h"
#include "ObjectMgr.h"
void Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
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
			packet.m_upacket.ph.time = timeGetTime();
			packet << 999 << "이진솔" << buffer;
			m_net.SendMsg(m_net.m_sock, packet.m_upacket);

			SendMessageA(m_edit, WM_SETTEXT, 0, (LPARAM)"");
		}break;
		}
	}break;
	}
	return m_net.MsgProc(hwnd, msg, wparam, lparam);
}
bool Sample::Init()
{
	////윈도우창 스타일
	//DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
	//m_edit = CreateWindow(L"edit", NULL, style, 0, g_rtClient.bottom - 50, 300, 50, m_hWnd, (HMENU)100, m_hInsatance, NULL);
	//style = WS_CHILD | WS_VISIBLE;
	//m_button = CreateWindow(L"button", L"전송", style, 310, g_rtClient.bottom - 50, 50, 50, m_hWnd, (HMENU)200, m_hInsatance, NULL);
	//m_listbox = CreateWindow(L"listbox", NULL, style, 0, 0, 300, g_rtClient.bottom - 70, m_hWnd, (HMENU)300, m_hInsatance, NULL);

	//SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)"채팅시작!");

	//for (int i = 0; i < 12; i++)
	//{
	//	DxObject obj;
	//	obj.Init();

	//	if (obj.Create(m_pd3dDevice, m_pImmediateContext, Vector2(-100 * i, i * 50), 400, 30))
	//	{
	//		m_ObjectList.push_back(obj);
	//	}
	//}

	I_Sound.Init();

	m_IntroWorld.Init();
	m_IntroWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_IntroWorld.m_pContext = m_pImmediateContext.Get();
	m_IntroWorld.Load(L"intro.txt");

	m_IntroWorld.m_pNextWorld1 = &m_GameWorld;
	m_IntroWorld.m_pNextWorld2 = &m_RankWorld;
	m_IntroWorld.m_pNextWorld3 = &m_UserserWorld;

	m_GameWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_GameWorld.m_pContext = m_pImmediateContext.Get();

	m_RankWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_RankWorld.m_pContext = m_pImmediateContext.Get();

	m_UserserWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_UserserWorld.m_pContext = m_pImmediateContext.Get();
	
	World::m_pWorld = &m_IntroWorld;

	m_net.Initnetwork();
	//"192.168.0.12"
	//"192.168.0.90"
	//"27.35.45.57"
	m_net.Connect(g_hwnd, SOCK_STREAM, 10000, "49.142.100.234");
	return true;
}
bool Sample::Frame()
{
	World::m_pWorld->Frame();

#pragma region
	int iChatCnt = m_net.m_playuser.m_packetpool.size();
	if (iChatCnt > 0 && m_chatcount != iChatCnt)
	{
		m_chatcount = iChatCnt;
		SendMessage(m_listbox, LB_RESETCONTENT, 0, 0);

		std::list<Packet>::iterator iter;
		if (m_net.m_playuser.m_packetpool.size() > 20)
		{
			m_net.m_playuser.m_packetpool.pop_front();
		}
		for (iter = m_net.m_playuser.m_packetpool.begin();
			iter != m_net.m_playuser.m_packetpool.end();
			iter++)
		{
			UPACKET& uPacket = (*iter).m_upacket;
			switch ((*iter).m_upacket.ph.type)
			{
			case PACKET_LOGIN_ACK:
			{
				DWORD dwCurrent = timeGetTime();
				DWORD dwEnd = 0;
				dwEnd = dwCurrent - uPacket.ph.time;
				LoginAck ack;
				memcpy(&ack, (*iter).m_upacket.msg, sizeof(LoginAck));
				if (ack.iResult == 1)
				{
					int k = 0;
				}
			}break;
			case PACKET_CHAT_MSG:
			{
				DWORD dwCurrent = timeGetTime();
				DWORD dwEnd = 0;
				dwEnd = dwCurrent - uPacket.ph.time;
				if (dwEnd >= 1)
				{
					std::string data = std::to_string(dwEnd);
					data += "\n";
					OutputDebugStringA(data.c_str());
				}
				ChatMsg recvdata;
				ZeroMemory(&recvdata, sizeof(recvdata));
				(*iter) >> recvdata.index >> recvdata.name
					>> recvdata.message;
				SendMessageA(m_listbox, LB_ADDSTRING, 0, (LPARAM)recvdata.message);
			}break;
			}
			//iter = m_Net.m_PlayerUser.m_packetPool.erase(iter);
			//(*iter).Reset();
		}
		m_net.m_playuser.m_packetpool.clear();
	}
#pragma endregion  NetProcess
	return true;
}
bool Sample::Render()
{
	World::m_pWorld->Render();

	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	//RECT a;
	//a.top = g_rtClient.bottom / 5;
	//a.left = g_rtClient.right / 5;
	//a.bottom = g_rtClient.bottom;
	//a.right = g_rtClient.right;
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(1, 1, 1, 1));
	return true;
}
bool Sample::Release()
{
	I_Sound.Release();
	m_IntroWorld.Release();
	m_GameWorld.Release();

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