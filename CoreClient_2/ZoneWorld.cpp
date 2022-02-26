#include "ZoneWorld.h"
#include "Input.h"
bool ZoneWorld::Init()
{
	return true;
}
bool ZoneWorld::Load(std::wstring file)
{
	Sound* pSound = I_Sound.Load("../../data/Sound/OnlyLove.MP3");
	pSound = I_Sound.Load("../../data/Sound/MyLove.MP3");
	m_pBackGroundMusic = I_Sound.Load("../../data/Sound/OnlyLove.MP3");
	pSound = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	pSound = I_Sound.Load("../../data/Sound/Gun1.wav");
	pSound = I_Sound.Load("../../data/Sound/abel_leaf.asf");
	pSound = I_Sound.Load("../../data/Sound/GunShot.mp3");
	//pSound = I_Sound.Load("../../data/Sound/pianoSound_00.MP3");
	//pSound = I_Sound.Load("../../data/Sound/romance.mid");
	m_pBackGroundMusic->Play(true);

	//플레이어 세팅
	m_PlayerObj.Init();
	m_PlayerObj.SetPosition(Vector2(400, 500));//속성값
	m_PlayerObj.SetRectSouce({ 91,1,42,56 });//속성값
	//m_PlayerObj.SetRectSouce({ 46,63,69,79 });
	m_PlayerObj.SetRectDraw({ 0,0, 42,56 });//속성값

	//플레이어 생성
	if (!m_PlayerObj.Create(m_pd3dDevice, m_pContext,
		L"Shader.txt",
		L"../../data/bitmap1.bmp",
		L"../../data/bitmap2.bmp"))
	{
		return false;
	}

	for (int iNpc = 0; iNpc < 10; iNpc++)
	{
		ObjectNpc2D* npc = new ObjectNpc2D;
		npc->Init();
		npc->SetPosition(Vector2(50 + iNpc * 150, 50));
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

		if (!npc->Create(m_pd3dDevice, m_pContext,
			L"Shader.txt",
			L"../../data/bitmap1.bmp",
			L"../../data/bitmap2.bmp"))
		{
			return false;
		}
		//ObjectNpc2D* -> 포인터가 아니면 push_back에 넣을때 복사가 안됨
		m_NpcLlist.push_back(npc);
	}
	return true;
}
bool ZoneWorld::Frame()
{
	if (Input::Get().GetKey(VK_F1) == KEY_PUSH)
	{
		Sound* pSound = I_Sound.GetPtr(L"GunShot.mp3");
		if (pSound != nullptr)
		{
			pSound->PlayEffect();
		}
	}

	m_pBackGroundMusic->Frame();

	if (Input::Get().GetKey(VK_UP) == KEY_HOLD)
	{
		m_pBackGroundMusic->VolumeUp(g_fSecPerFrame);
	}
	if (Input::Get().GetKey(VK_DOWN) == KEY_HOLD)
	{
		m_pBackGroundMusic->VolumeDown(g_fSecPerFrame);
	}

	m_PlayerObj.Frame();
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Frame();
	}
	return true;
}
bool ZoneWorld::Render()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		if (m_NpcLlist[iObj]->m_bDead == false)
		{
			m_NpcLlist[iObj]->Render();
		}
	}
	m_PlayerObj.Render();

	//RECT rt = g_rtClient;
	//rt.top = 300;
	//rt.left = 0;
	//m_dxWrite.Draw(m_pBackGroundMusic->m_szBuffer, rt, D2D1::ColorF(0, 0, 0, 1),
	//	m_dxWrite.m_pd2dMTShadowTF);
	return true;
}
bool ZoneWorld::Release()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
		delete m_NpcLlist[iObj];
	}
	m_PlayerObj.Release();
	m_NpcLlist.clear();
	return true;
}