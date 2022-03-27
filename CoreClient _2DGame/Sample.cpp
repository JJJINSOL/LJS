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
	I_Sound.Init();

	m_IntroWorld.Init();
	m_IntroWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_IntroWorld.m_pContext = m_pImmediateContext.Get();
	m_IntroWorld.Load(L"intro.txt");

	m_IntroWorld.m_pNextWorld1 = &m_GameWorld;
	m_IntroWorld.m_pNextWorld2 = &m_RankWorld;
	m_IntroWorld.m_pNextWorld3 = &m_UserserWorld;

	m_GameWorld.Init();
	m_GameWorld.World::m_pd3dDevice = m_pd3dDevice.Get();
	m_GameWorld.World::m_pContext = m_pImmediateContext.Get();
	m_GameWorld.m_pNextWorld = &m_IntroWorld;

	m_RankWorld.Init();
	m_RankWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_RankWorld.m_pContext = m_pImmediateContext.Get();
	m_RankWorld.Load(L"intro.txt");

	m_UserserWorld.Init();
	m_UserserWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_UserserWorld.m_pContext = m_pImmediateContext.Get();
	m_UserserWorld.Load(L"intro.txt");
	
	m_ResultWorld.Init();
	m_ResultWorld.m_pd3dDevice = m_pd3dDevice.Get();
	m_ResultWorld.m_pContext = m_pImmediateContext.Get();
	/*m_ResultWorld.Load(L"intro.txt");*/

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
	if (World::m_pWorld->btnintro == true)
	{
		World::m_pWorld->btnintro = false;
		//I_ObjectMgr.Release();
		//m_ResultWorld.Release();
		//m_IntroWorld.Init();
		//m_IntroWorld.m_pd3dDevice = m_pd3dDevice.Get();
		//m_IntroWorld.m_pContext = m_pImmediateContext.Get();
		//m_IntroWorld.Load(L"intro.txt");
		//World::m_pWorld = &m_IntroWorld;
		//World::m_pWorld->m_bLoadZone = false;
		
	}

	if (World::m_pWorld->btnrule == true)
	{
		I_ObjectMgr.Release();
		World::m_pWorld = &m_IntroWorld;
		World::m_pWorld->btnrule = false;
	}
	if (m_GameWorld.m_PlayerObj.m_life <= 0)
	{
		World::m_pWorld->btngame = false;
		m_GameWorld.m_gamestart = false;
		m_GameWorld.m_PlayerObj.m_life = 3;
		m_GameWorld.m_pBackGroundMusic->Stop();
		World::m_pWorld->m_bLoadZone = false;

		m_GameWorld.m_score = (int)m_GameTimer.m_fTimer * 100;
		I_ObjectMgr.Release();
		m_GameWorld.Release();
		//I_ObjectMgr.Release();
		//World::m_pWorld->hardmode = false;
		//m_GameWorld.hard = false;
		m_ResultWorld.Load(L"intro.txt");
		m_ResultWorld.m_pBackGroundMusic->Play(false);
		World::m_pWorld = &m_ResultWorld;
		//World::m_pWorld->m_bLoadZone = true;
	}
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
	if (World::m_pWorld == (World*)&m_GameWorld)
	{
		if (m_GameWorld.m_gamestart == false)
		{
			m_GameTimer.m_fTimer = 0;
			m_GameWorld.m_gamestart = true;
		}
		std::wstring msg = L"SCORE  ";
		msg += std::to_wstring((int)m_GameTimer.m_fTimer*100);
		RECT a;
		a.top = 30;
		a.left = 750;
		a.bottom = g_rtClient.bottom;
		a.right = g_rtClient.right;
		if (m_colortime == 0)
		{
			m_dxWrite.Draw(msg, a, D2D1::ColorF(1, 1, 1, 1));
		}
		else if(m_colortime!=0)
		{
			m_checktime += g_fSecPerFrame;
			m_dxWrite.Draw(msg, a, D2D1::ColorF(1, 0, 0, 1));
		}
		if (m_checktime >= m_colortime)
		{
			m_colortime = 0;
		}
		if (m_GameWorld.m_plusescore == true)
		{
			std::wstring msg = L"SCORE  ";
			m_GameTimer.m_fTimer += 10;
			msg += std::to_wstring((int)m_GameTimer.m_fTimer * 100);
			m_dxWrite.Draw(msg, a, D2D1::ColorF(1, 0, 0, 1));
			m_GameWorld.m_plusescore = false;
			m_colortime = 1.5;
			m_checktime = g_fSecPerFrame;
		}
	}
	else if (World::m_pWorld == (World*)&m_ResultWorld)
	{
		std::wstring msg = L"SCORE  ";
		msg += std::to_wstring(m_GameWorld.m_score);
		RECT a;
		a.top = 250;
		a.left = 0;
		a.bottom = g_rtClient.bottom;
		a.right = g_rtClient.right;
		m_dxWrite.Draw(msg, a, D2D1::ColorF(0, 0, 0, 1));
	}

	return true;
}
bool Sample::Release()
{
	I_Sound.Release();
	m_IntroWorld.Release();
	m_GameWorld.Release();
	m_ResultWorld.Release();
	m_UserserWorld.Release();

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