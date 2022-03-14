#include "GameWorld.h"
#include "Input.h"
#include "ObjectMgr.h"
bool GameWorld:: CreateNPC()
{
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	for (int iNpc = 0; iNpc < 20; iNpc++)
	{
		//std::shared_ptr<ObjectNpc2D> npc = std::make_shared<ObjectNpc2D>();
		ObjectNpc2D* npc = new ObjectNpc2D;
		npc->m_csName = L"NPC";
		//npc->m_csName += std::to_wstring(iNpc);
		npc->Init();

		npc->SetRectDraw({ 0,0, 25,25 });
		npc->SetPosition(Vector2(20 + (rand() % 960), 0));

		npc->m_pColorTex = I_Texture.Load(L"../../DX2D/data/star_01.png");
		npc->m_pMaskTex = nullptr;
		npc->m_pVShader = pVShader;
		npc->m_pPShader = pPShader;

		if (!npc->Create(World::m_pd3dDevice, m_pContext))
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
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	std::shared_ptr<ImageObject> obj(new ImageObject);
	obj->m_csName = L"ImageObject:BG";
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(Vector2(0, 0));
	obj->m_pColorTex = m_BGTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;
	if (!obj->Create(World::m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"bg", obj));
}
bool GameWorld::Init()
{
	return true;
}
bool GameWorld::Load(std::wstring file)
{
	m_BGTex = I_Texture.Load(L"../../DX2D/data/bg.png");
	//Sound* pSound = I_Sound.Load("../../data/Sound/OnlyLove.MP3");
	//pSound = I_Sound.Load("../../data/Sound/MyLove.MP3");
	m_pBackGroundMusic = I_Sound.Load("../../DX2D/data/BGM.mp3");
	Sound* pSound = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	pSound = I_Sound.Load("../../data/Sound/Gun1.wav");
	pSound = I_Sound.Load("../../data/Sound/abel_leaf.asf");
	m_peffect = I_Sound.Load("../../data/Sound/GunShot.mp3");

	pSound = I_Sound.Load("../../DX2D/data/eff.wav");

	m_pBackGroundMusic->Play(true);

	CreateModelType();

	//배경 세팅
	Object2D* pNewBK = I_UI.GetPtr(L"bg")->Clone();
	pNewBK->m_csName = L"TImageObjectClock:bk";
	pNewBK->SetPosition(Vector2(500, 250));
	pNewBK->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBK));

	//Life
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	m_Life.m_csName = L"Life";
	m_Life.Init();

	m_Life.SetRectDraw({ 0,0, 170,50 });
	m_Life.SetPosition(Vector2(105,45));

	m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/heart_3.png");
	m_Life.m_pMaskTex = nullptr;
	m_Life.m_pVShader = pVShader;
	m_Life.m_pPShader = pPShader;

	if (!m_Life.Create(World::m_pd3dDevice, m_pContext))
	{
		return false;
	}
	m_Life.SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);

	//플레이어 세팅----------------------------------------------------
	m_PlayerObj.m_csName = L"PlayerUser";
	m_PlayerObj.Init();
	m_PlayerObj.SetPosition(Vector2(300, 450));
	m_PlayerObj.SetRectSouce({ 418,634,102,144 });
	m_PlayerObj.SetRectDraw({ 300, 460, 40,50 });
	m_PlayerObj.SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
	
	//플레이어 생성
	if (!m_PlayerObj.Create(World::m_pd3dDevice, m_pContext, L"Shader.txt",	L"../../DX2D/data/player.bmp",	L"../../DX2D/data/playerback.bmp"))
	{
		return false;
	}
	m_PlayerObj.UpdateData();
	//m_PlayerObj.m_life = 3;
	CreateNPC();
	
	I_Sprite.Load(L"SpriteData.txt");
	//World::m_pWorld = m_pNextWorld;
	return true;
}
bool GameWorld::Frame()
{
	m_Life.Frame();
	m_PlayerObj.Frame();

	if (m_PlayerObj.m_life <= 0)
	{
		//m_pBackGroundMusic->Stop();
		m_pNextWorld->Load(L"world.txt");
		I_ObjectMgr.Release();
		//World::m_pWorld = &m_IntroWorld1;
		//World::m_pWorld->m_bLoadZone = true;
	}
	if (m_PlayerObj.m_life ==1)
	{
		
		m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/heart_1.png");
		
	}
	if (m_PlayerObj.m_life == 2)
	{
		m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/heart_2.png");
		
	}
	if (m_PlayerObj.m_life >= 3)
	{
		m_Life.m_pColorTex = I_Texture.Load(L"../../DX2D/data/heart_3.png");
		m_PlayerObj.m_life = 3;
		
	}


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
				iter = m_npclist.erase(iter);
			}
			else if ((*iter)->m_bDead)
			{
				delete (*iter);
				iter = m_npclist.erase(iter);
				//m_peffect->PlayEffect();
				m_PlayerObj.m_life--;
			}
			else
			{
				m_fStarTimer += g_fSecPerFrame;
				if (m_fStarTimer >= m_fStarChangeTime)
				{
					m_iStarCurrentIndex++;
					if (m_iStarCurrentIndex >= 4)
					{
						m_iStarCurrentIndex = 0;
					}
					m_fStarTimer -= m_fStarChangeTime;
				}
				if (m_iStarCurrentIndex == 0)
				{
					(*iter)->m_pColorTex = I_Texture.Load(L"../../DX2D/data/star_01.png");
				}
				else if (m_iStarCurrentIndex == 1)
				{
					(*iter)->m_pColorTex = I_Texture.Load(L"../../DX2D/data/star_02.png");
				}
				else if (m_iStarCurrentIndex == 2)
				{
					(*iter)->m_pColorTex = I_Texture.Load(L"../../DX2D/data/star_03.png");
				}
				else if (m_iStarCurrentIndex == 3)
				{
					(*iter)->m_pColorTex = I_Texture.Load(L"../../DX2D/data/star_02.png");
				}
				(*iter)->Frame();
				iter++;
			}
		}
	}
	static int Lindex = 1;
	if (Input::Get().GetKey(VK_LEFT) == KEY_HOLD)
	{
		if (Lindex == 1)
		{
			m_PlayerObj.SetRectSouce({ 18,432,102,144 });
			Lindex = 2;
		}
		else if (Lindex == 2)
		{
			m_PlayerObj.SetRectSouce({ 150,432,102,144 });
			Lindex = 3;
		}
		else if (Lindex == 3)
		{
			m_PlayerObj.SetRectSouce({ 285,432,102,144 });
			Lindex = 4;
		}
		else if (Lindex == 4)
		{
			m_PlayerObj.SetRectSouce({ 418,432,102,144 });
			Lindex = 1;
		}
	}
	static int Rindex = 1;
	if (Input::Get().GetKey(VK_RIGHT) == KEY_HOLD)
	{
		if (Rindex == 1)
		{
			m_PlayerObj.SetRectSouce({ 418,634,102,144 });
			Rindex = 2;
		}
		else if (Rindex == 2)
		{
			m_PlayerObj.SetRectSouce({ 285,634,102,144 });
			Rindex = 3;
		}
		else if (Rindex == 3)
		{
			m_PlayerObj.SetRectSouce({ 150,634,102,144 });
			Rindex = 4;
		}
		else if (Rindex == 4)
		{
			m_PlayerObj.SetRectSouce({ 18,634,102,144 });
			Rindex = 1;
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
	m_Life.Render();

	//std::wstring msg = L"SCORE  ";
	//msg += std::to_wstring((int)m_time.m_fTimer*100);

	//m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(1, 1, 1, 1));
	return true;
}
bool GameWorld::Release()
{
	m_PlayerObj.Release();
	m_Life.Release();
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