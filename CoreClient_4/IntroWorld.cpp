#include "IntroWorld.h"
#include "Input.h"
#include <string>
#include "ObjectMgr.h"
#include "UIModelMgr.h"
bool IntroWorld::CreateModelType()
{
	/// 배경이미지---------------------------------------------------------------
	Shader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	ImageObject* obj = new ImageObject;
	obj->m_csName = L"TImageObject:bk";
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(Vector2(0, 0));
	obj->m_pColorTex = m_pColorTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;
	if (!obj->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"bk", obj));

	/// 대화상자--------------------------------------------------------------
	ButtonObject* btnDlg = new ButtonObject;
	btnDlg->m_csName = L"TButtonObject:btnDlg";
	btnDlg->Init();
	btnDlg->m_rtOffset = { 50, 50, 50, 50 };
	btnDlg->SetRectDraw({ 0,0, g_rtClient.right / 3,g_rtClient.bottom / 3 });
	btnDlg->SetPosition(Vector2(0, 0));
	if (!btnDlg->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../data/ui/popup_normal.png"))
	{
		return false;
	}
	btnDlg->SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnDlg", btnDlg));

	/// 버튼0----------------------------------------------------------------
	ButtonObject* btnObj = new ButtonObject;
	btnObj->m_csName = L"TButtonObject:btnStart";
	btnObj->Init();
	btnObj->m_rtOffset = { 0, 0, 0, 0 };
	btnObj->SetRectDraw({ 0,0, 334,82 });
	btnObj->SetPosition(Vector2(0,0));
	Texture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	Sound* pSound = I_Sound.Load("../../data/Sound/00_Menu.MP3");

	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);
	pTex = I_Texture.Load(L"../../data/ui/main_start_pus.png");
	pSound = I_Sound.Load("../../data/Sound/FootStepSound.wav");

	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);
	pTex = I_Texture.Load(L"../../data/ui/main_start_sel.png");
	pSound = I_Sound.Load("../../data/Sound/FootStepSound_2.wav");

	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);
	pTex = I_Texture.Load(L"../../data/ui/main_start_dis.png");
	pSound = I_Sound.Load("../../data/Sound/PianoSound_00.mp3");

	// 가변인자를 통해서 생성자 직접 호출
	btnObj->m_pStatePlayList.emplace_back(pTex, pSound);

	if (!btnObj->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../data/main_start_nor.png"))
	{
		return false;
	}
	btnObj->SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnStart", btnObj));

	// 새로운 모델을 생성해서 등록한다.
	UIModelComposed* pComposedObj = new UIModelComposed;
	pComposedObj->m_csName = L"TUIModelComposed";
	ButtonObject* pDlgWindow = (ButtonObject*)I_UI.GetPtr(L"btnDlg")->Clone();
	pDlgWindow->m_pParent = nullptr;
	pDlgWindow->m_rtOffset = { 50, 50, 50, 50 };
	pDlgWindow->SetRectDraw({ 0,0, g_rtClient.right / 3,g_rtClient.bottom / 3 });
	pDlgWindow->SetPosition(Vector2(400, 300));
	pComposedObj->Add(pDlgWindow);
	UIModel* pNewDlgBtn = I_UI.GetPtr(L"btnStart")->Clone();// new TButtonObject(*I_UI.GetPtr(L"btnStart"));
	pNewDlgBtn->m_csName = L"btnStartClone_ComposedList";
	pNewDlgBtn->m_pParent = pDlgWindow;
	pNewDlgBtn->SetRectDraw({ 0,0, 100,50 });
	pNewDlgBtn->SetPosition(pDlgWindow->m_vPos + Vector2(0, 0));
	pComposedObj->Add(pNewDlgBtn);
	I_UI.m_list.insert(std::make_pair(L"dlgWindow", pComposedObj));
	return true;
}
bool IntroWorld::Init()
{
	World::Init();
	//m_pBackGroundMusic->Play(true);
	return true;
}
bool IntroWorld::Load(std::wstring file)
{
	m_pBackGroundMusic = I_Sound.Load("../../data/Sound/OnlyLove.MP3");
	m_pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");

	for (int i = 0; i < 10; i++)
	{
		std::wstring name = L"../../data/";
		name += std::to_wstring(i);
		name += L".bmp";
		m_pChangeColorTex[i] = I_Texture.Load(name);
	}
	CreateModelType();

	UIModel* pNewBK = I_UI.GetPtr(L"bk")->Clone();
	pNewBK->m_csName = L"ImageObjectClock:bk";
	pNewBK->AddPosition(Vector2(400, 300));
	pNewBK->UpdateData();
	m_UIObj.push_back(pNewBK);
	// 프로토타입 디자인 패턴-> 복제를 통해서 객체 생성/ + 컴포짓(Composite패턴)
	UIModel* pNewBtn = I_UI.GetPtr(L"btnStart")->Clone();
	pNewBtn->m_csName = L"btnStartClone1";
	pNewBtn->SetRectDraw({ 0,0, 100,50 });
	pNewBtn->AddPosition(Vector2(300, 25));
	pNewBtn->UpdateData();
	m_UIObj.push_back(pNewBtn);

	UIModel* pNewBtn2 = I_UI.GetPtr(L"btnStart")->Clone();
	pNewBtn2->m_csName = L"btnStartClone2";
	pNewBtn2->SetRectDraw({ 0,0, 100,100 });
	pNewBtn2->AddPosition(Vector2(400, 150));
	pNewBtn2->UpdateData();
	m_UIObj.push_back(pNewBtn2);

	pNewBtn = I_UI.GetPtr(L"btnStart")->Clone();
	pNewBtn->m_csName = L"btnStartClone3";
	pNewBtn->SetRectDraw({ 0,0, 100,50 });
	pNewBtn->AddPosition(Vector2(500, 200));
	pNewBtn->UpdateData();
	m_UIObj.push_back(pNewBtn);

	UIModel* pNewDlgBtnClone = I_UI.GetPtr(L"dlgWindow")->Clone();
	pNewDlgBtnClone->m_csName = L"UIModelComposedClone";
	pNewDlgBtnClone->m_pParent = nullptr;
	pNewDlgBtnClone->SetPosition(Vector2(0, 0));
	pNewDlgBtnClone->UpdateData();
	m_UIObj.push_back(pNewDlgBtnClone);

	ListCtrlObject* pListCtrl = new ListCtrlObject;
	pListCtrl->m_csName = L"ListCtrlObject";
	pListCtrl->SetRectDraw({ 100,100, 700,10 });
	pListCtrl->Create(100, 1);
	pListCtrl->UpdateData();
	m_UIObj.push_back(pListCtrl);

	// 이펙트========================================================
	I_Sprite.Load(L"SpriteData.txt");
	m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtExplosion");

	m_ExplosionObj.Init();
	m_ExplosionObj.SetPosition(Vector2(400, 500));
	Sprite* pSprite = I_Sprite.GetPtr(L"rtExplosion");
	m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
	m_ExplosionObj.SetRectDraw({ 0,0, 34, 46 });

	if (!m_ExplosionObj.Create(m_pd3dDevice, m_pContext,
		L"Shader.txt",
		L"../../data/bitmap1.bmp",
		L"../../data/bitmap2.bmp"))
	{
		return false;
	}

	return true;
}
bool IntroWorld::Frame()
{
	if (m_bLoadZone && m_pNextWorld != nullptr)
	{
		I_ObjectMgr.Release();
		m_pNextWorld->Load(L"zone.txt");
		World::m_pWorld = m_pNextWorld;
	}
	m_pBackGroundMusic->Frame();
	static int iIndex = 0;
	//if (Input::Get().GetKey(VK_F2) == KEY_PUSH)
	//{
	//	iIndex++;
	//	if (iIndex >= 10)
	//	{
	//		iIndex = 0;
	//	}
	//	auto data = m_UIObj.find(L"bk");
	//	if (data != m_UIObj.end())
	//	{
	//		data->second->m_pColorTex = m_pChangeColorTex[iIndex];
	//	}
	//}
	if (Input::Get().GetKey(VK_F3) == KEY_PUSH)
	{
		iIndex++;
		if (iIndex >= m_ExplosionObj.m_pSprite->m_rtArray.size())
		{
			iIndex = 0;
		}
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[iIndex]);
		m_ExplosionObj.SetRectDraw({ 0,0,
			m_ExplosionObj.m_pSprite->m_rtArray[iIndex].right,
			m_ExplosionObj.m_pSprite->m_rtArray[iIndex].bottom });
	}
	if (Input::Get().GetKey(VK_F4) == KEY_PUSH)
	{
		m_ExplosionObj.Reset();
		m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtExplosion");
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
		m_ExplosionObj.SetRectDraw({ 0,0,
			m_ExplosionObj.m_pSprite->m_rtArray[0].right,
			m_ExplosionObj.m_pSprite->m_rtArray[0].bottom });
	}
	if (Input::Get().GetKey(VK_F6) == KEY_PUSH)
	{
		m_ExplosionObj.Reset();
		m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtBomb");
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
		m_ExplosionObj.SetRectDraw({ 0,0,
			m_ExplosionObj.m_pSprite->m_rtArray[0].right,
			m_ExplosionObj.m_pSprite->m_rtArray[0].bottom });
	}
	if (Input::Get().GetKey(VK_F7) == KEY_PUSH)
	{
		m_ExplosionObj.Reset();
		m_ExplosionObj.m_pSprite = I_Sprite.GetPtr(L"rtClash");
		m_ExplosionObj.SetRectSouce(m_ExplosionObj.m_pSprite->m_rtArray[0]);
		m_ExplosionObj.SetRectDraw({ 0,0,
			m_ExplosionObj.m_pSprite->m_rtArray[0].right,
			m_ExplosionObj.m_pSprite->m_rtArray[0].bottom });
	}
	m_ExplosionObj.Frame();
	World::Frame();
	return true;
}
bool IntroWorld::Render()
{
	World::Render();
	m_ExplosionObj.Render();
	return true;
}
bool IntroWorld::Release()
{
	World::Release();
	m_ExplosionObj.Release();
	return true;
}