#include "GameWorld.h"
#include "Input.h"
#include "ObjectMgr.h"
bool GameWorld:: CreateNPC()
{
	Shader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	for (int iNpc = 0; iNpc < 20; iNpc++)
	{
		//std::shared_ptr<ObjectNpc2D> npc = std::make_shared<ObjectNpc2D>();
		ObjectNpc2D* npc = new ObjectNpc2D;
		npc->m_csName = L"NPC";
		//npc->m_csName += std::to_wstring(iNpc);
		npc->Init();

		npc->SetRectDraw({ 0,0, 30,30 });
		npc->SetPosition(Vector2(20 + (rand() % 960), 0));

		npc->m_pColorTex = I_Texture.Load(L"../../DX2D/data/red_star_01.png");
		npc->m_pMaskTex = nullptr;
		npc->m_pVShader = pVShader;
		npc->m_pPShader = pPShader;

		if (!npc->Create(m_pd3dDevice, m_pContext))
		{
			return false;
		}
		npc->SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
		npc->UpdateData();
		//m_NpcObj.push_back(npc);
		m_npclist.push_back(npc);
	}
}
bool GameWorld::CreateModelType()
{
	/// 배경이미지---------------------------------------------------------------
	Shader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	std::shared_ptr<ImageObject> obj(new ImageObject);
	obj->m_csName = L"ImageObject:BG";
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(Vector2(0, 0));
	obj->m_pColorTex = m_BGTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;
	if (!obj->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"bg", obj));

	return true;
}
bool GameWorld::Init()
{
	return true;
}
bool GameWorld::Load(std::wstring file)
{
	m_BGTex = I_Texture.Load(L"../../DX2D/data/bg.png");
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

	CreateModelType();

	Object2D* pNewBK = I_UI.GetPtr(L"bg")->Clone();
	pNewBK->m_csName = L"TImageObjectClock:bk";
	pNewBK->SetPosition(Vector2(500, 250));
	pNewBK->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBK));

	//플레이어 세팅----------------------------------------------------
	m_PlayerObj.m_csName = L"PlayerUser";
	m_PlayerObj.Init();
	m_PlayerObj.SetPosition(Vector2(300, 450));
	m_PlayerObj.SetRectSouce({ 418,634,102,144 });
	m_PlayerObj.SetRectDraw({ 300, 450, 40,50 });
	m_PlayerObj.SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
	

	//플레이어 생성
	if (!m_PlayerObj.Create(m_pd3dDevice, m_pContext,
		L"Shader.txt",
		L"../../DX2D/data/player.bmp",
		L"../../DX2D/data/playerback.bmp"))
	{
		return false;
	}
	m_PlayerObj.UpdateData();
	CreateNPC();
	return true;
}
bool GameWorld::Frame()
{
	m_PlayerObj.Frame();

	vector<ObjectNpc2D*>::iterator iter;
	for (iter = m_npclist.begin(); iter != m_npclist.end(); )
	{
		if (m_npclist.size() < 9)
		{
			CreateNPC();
		}
		if ((*iter) != nullptr)
		{
			
			if ((*iter)->m_vPos.y > 450)
			{
				delete (*iter);
				//*iter = nullptr;
				iter = m_npclist.erase(iter);
			}
			else
			{
				(*iter)->Frame();
				iter++;
			}
		}
	}

	if (Input::Get().GetKey('Z') == KEY_PUSH)
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

	World::Frame();



	return true;
}
bool GameWorld::Render()
{
	World::Render();
	vector<ObjectNpc2D*>::iterator iter;
	for (iter = m_npclist.begin(); iter != m_npclist.end(); iter++)
	{
		//ObjectNpc2D* pObj = obj;
		if ((*iter) != nullptr)
		{
			(*iter)->Render();
		}
	}
	m_PlayerObj.Render();
	//std::wstring msg = L"FPS:";
	//msg += std::to_wstring(m_GameTimer.m_iFPS);
	//msg += L"  GT:";
	//msg += std::to_wstring(m_GameTimer.m_fTimer);
	////RECT a;
	////a.top = g_rtClient.bottom / 5;
	////a.left = g_rtClient.right / 5;
	////a.bottom = g_rtClient.bottom;
	////a.right = g_rtClient.right;
	//m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(1, 1, 1, 1));
	return true;
}
bool GameWorld::Release()
{
	m_PlayerObj.Release();
	vector<ObjectNpc2D*>::iterator iter;
	for (iter = m_npclist.begin(); iter != m_npclist.end(); iter++)
	{
		//ObjectNpc2D* pObj = obj;
		if ((*iter) != nullptr)
		{
			(*iter)->Release();
		}
	}
	return true;
}