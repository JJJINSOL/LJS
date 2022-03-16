#include "ResultWorld.h"
#include "ObjectMgr.h"
#include "UIModelMgr.h"
bool ResultWorld::CreateModelType()
{
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");

	std::shared_ptr<ImageObject> obj1(new ImageObject);
	obj1->m_csName = L"ImageObject:BGSub";
	obj1->Init();
	obj1->SetRectDraw({ 0,0, 900,400 });
	obj1->SetPosition(Vector2(0, 0));
	obj1->m_pColorTex = m_BGSubTex;
	obj1->m_pMaskTex = nullptr;
	obj1->m_pVShader = pVShader;
	obj1->m_pPShader = pPShader;
	if (!obj1->Create(World::m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj1->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"bgsub", obj1));

	/// 버튼---------------------------------------------------------------
	std::shared_ptr<ButtonObject> btnhome(new ButtonObject);
	btnhome->m_csName = L"TButtonObject:btnreplay";
	btnhome->Init();
	btnhome->m_rtOffset = { 0, 0, 0, 0 };
	btnhome->SetRectDraw({ 0,0, 340,100 });//334,82//340,100
	btnhome->SetPosition(Vector2(0, 0));

	Texture* pTex1 = I_Texture.Load(L"../../DX2D/data/replay_nor.png");
	Sound* pSound1 = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	btnhome->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/replay_hover.png");
	pSound1 = I_Sound.Load("../../data/Sound/FootStepSound.wav");
	btnhome->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/replay_sel.png");
	pSound1 = I_Sound.Load("../../data/Sound/FootStepSound_2.wav");
	btnhome->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/replay_nor.png");
	pSound1 = I_Sound.Load("../../data/Sound/PianoSound_00.mp3");
	btnhome->m_pStatePlayList.emplace_back(pTex1, pSound1);

	if (!btnhome->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../DX2D/data/replay_nor.png"))
	{
		return false;
	}
	btnhome->SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnreplay", btnhome));

	return true;
}
bool ResultWorld::Init()
{
	World::Init();
	return true;
}
bool ResultWorld::Load(std::wstring file)
{
	m_BGTex = I_Texture.Load(L"../../DX2D/data/bg.png");
	m_BGSubTex = I_Texture.Load(L"../../DX2D/data/white.png");

	m_pBackGroundMusic = I_Sound.Load("../../DX2D/data/END.MP3");
	
	CreateModelType();

	//배경 세팅
	Object2D* pNewBK = I_UI.GetPtr(L"bg")->Clone();
	pNewBK->m_csName = L"TImageObjectClock:bk";
	pNewBK->SetPosition(Vector2(500, 250));
	pNewBK->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBK));

	Object2D* pNewBKSub = I_UI.GetPtr(L"bgsub")->Clone();
	pNewBKSub->m_csName = L"TImageObjectClock:bksub";
	pNewBKSub->SetPosition(Vector2(500, 250));
	pNewBKSub->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBKSub));

	Object2D* pNewbtn = I_UI.GetPtr(L"btnreplay")->Clone();//btnreplay
	pNewbtn->m_csName = L"replay";
	pNewbtn->SetPosition(Vector2(500, 370));
	pNewbtn->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewbtn));

	//=========================================================
	Shader* pVShader = I_Shader.CreateVertexShader(World::m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(World::m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	m_Gameover.m_csName = L"Gameover";
	m_Gameover.Init();

	m_Gameover.SetRectDraw({ 0,0, 600,150 });
	m_Gameover.SetPosition(Vector2(500, 130));

	m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/Go_1.png");
	m_Gameover.m_pMaskTex = nullptr;
	m_Gameover.m_pVShader = pVShader;
	m_Gameover.m_pPShader = pPShader;

	if (!m_Gameover.Create(World::m_pd3dDevice, m_pContext))
	{
		return false;
	}
	m_Gameover.SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);


	return true;
}
bool ResultWorld::Frame()
{
	
	m_pBackGroundMusic->Frame();
	m_Gameover.Frame();
	World::Frame();

	m_fGOTimer += g_fSecPerFrame;
	if (m_fGOTimer >= m_fGOChangeTime)
	{
		m_CurrentIndex++;
		if (m_CurrentIndex >= 8)
		{
			m_CurrentIndex = 0;
		}
		m_fGOTimer -= m_fGOChangeTime;
	}
	if (m_CurrentIndex == 0)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_1.png");
	}
	else if (m_CurrentIndex == 1)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_2.png");
	}
	else if (m_CurrentIndex == 2)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_3.png");
	}
	else if (m_CurrentIndex == 3)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_4.png");
	}
	else if (m_CurrentIndex == 4)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_5.png");
	}
	else if (m_CurrentIndex == 5)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_6.png");
	}
	else if (m_CurrentIndex == 6)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_7.png");
	}
	else if (m_CurrentIndex == 7)
	{
		m_Gameover.m_pColorTex = I_Texture.Load(L"../../DX2D/data/GO_8.png");
	}


	return true;
}
bool ResultWorld::Render()
{
	World::Render();
	m_Gameover.Render();
	return true;
}
bool ResultWorld::Release()
{
	World::Release();
	m_Gameover.Release();
	return true;
}