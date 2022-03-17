#include "Sample.h"
#include "Fsm.h"
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
	I_Fsm.AddTransition(STATE_STOP, STATE_STOP_TIMER, STATE_MOVE);
	I_Fsm.AddTransition(STATE_STOP, STATE_FIND_TARGET, STATE_ATTACK);

	I_Fsm.AddTransition(STATE_MOVE, STATE_STOP_TIMER, STATE_STOP);
	I_Fsm.AddTransition(STATE_MOVE, STATE_FIND_TARGET, STATE_ATTACK);
	I_Fsm.AddTransition(STATE_MOVE, STATE_LOST_TARGET, STATE_STOP);

	I_Fsm.AddTransition(STATE_ATTACK, STATE_LOST_TARGET, STATE_STOP);

	Shader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice.Get(), L"VertexShader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(), L"PixelShader.txt", "PS");

	m_PlayerObj.Init();
	m_PlayerObj.SetRectSouce({ 91,1,42,56 });
	//m_PlayerObj.SetRectSouce({ 46,63,69,79 });
	m_PlayerObj.SetRectDraw({ 0,0, 42,56 });
	m_PlayerObj.SetPosition(Vector2(400, 300));
	m_PlayerObj.m_pVShader = pVShader;
	m_PlayerObj.m_pPShader = pPShader;

	if (!m_PlayerObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),
		nullptr,L"../../data/bitmap1.bmp",L"../../data/bitmap2.bmp"))
	{
		return false;
	}

	for (int iNpc = 0; iNpc < 1; iNpc++)
	{
		std::shared_ptr<ObjectNpc2D> npc = std::make_shared<ObjectNpc2D>();
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

		npc->SetPosition(Vector2(50 + iNpc * 150, 50));

		npc->m_pVShader = pVShader;
		npc->m_pPShader = pPShader;

		if (!npc->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),
			nullptr,L"../../data/bitmap1.bmp",L"../../data/bitmap2.bmp"))
		{
			return false;
		}
		m_NpcLlist.push_back(npc);
	}



	m_net.Initnetwork();
	//"192.168.0.12"
	//"192.168.0.90"
	//"27.35.45.57"
	m_net.Connect(g_hwnd, SOCK_STREAM, 10000, "211.47.119.43");
	return true;
}
bool Sample::Frame()
{
	m_PlayerObj.Frame();

	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Frame();
		m_NpcLlist[iObj]->m_pStateAction->Process(&m_PlayerObj);
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