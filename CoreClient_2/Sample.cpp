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
			packet << 999 << "이진솔" << 50 << buffer;
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

	//플레이어 세팅
	m_PlayerObj.Init();
	m_PlayerObj.SetPosition(Vector2(400, 300));//속성값
	m_PlayerObj.SetRectSouce({ 91,1,42,56 });//속성값
	//m_PlayerObj.SetRectSouce({ 46,63,69,79 });
	m_PlayerObj.SetRectDraw({ 0,0, 42,56 });//속성값
	
	//생성
	if (!m_PlayerObj.Create(m_pd3dDevice, m_pImmediateContext,
		L"../../data/bitmap1.bmp",
		L"../../data/bitmap2.bmp"))
	{
		return false;
	}

	for (int iNpc = 0; iNpc < 10; iNpc++)
	{
		
		ObjectNpc2D* npc = new ObjectNpc2D;
		npc->Init();
		if (iNpc % 2 == 0)
		{
			npc->SetRectSouce({ 46,63,69,79 });
			npc->SetRectDraw({ 0,0, 69,79 });
		}
		else
		{
			npc->SetRectSouce({ 1,63,42,76 });
			npc->SetRectDraw({ 0,0, 42,76 });
		}

		npc->SetPosition(Vector2(50 + iNpc * 50, 50));
		if (!npc->Create(m_pd3dDevice, m_pImmediateContext,
			L"../../data/bitmap1.bmp",
			L"../../data/bitmap2.bmp"))
		{
			return false;
		}
		//ObjectNpc2D* -> 포인터가 아니면 push_back에 넣을때 복사가 안됨
		m_NpcLlist.push_back(npc);
	}



	m_net.Initnetwork();
	//"192.168.0.12"
	//"192.168.0.90"
	//"27.35.45.57"
	m_net.Connect(g_hwnd, SOCK_STREAM, 10000, "120.50.65.157");
	return true;
}
bool Sample::Frame()
{
	m_PlayerObj.Frame();

	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Frame();
	}

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
	return true;
}
bool Sample::Render()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Render();
	}
	m_PlayerObj.Render();
	return true;
}
bool Sample::Release()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
	}
	m_PlayerObj.Release();

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