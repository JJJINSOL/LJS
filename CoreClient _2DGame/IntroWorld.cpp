#include "IntroWorld.h"
#include "Input.h"
#include <string>
#include "ObjectMgr.h"
#include "UIModelMgr.h"
extern std::shared_ptr<ButtonObject> btnObj1(new ButtonObject);
extern std::shared_ptr<ButtonObject> btnObj2(new ButtonObject);
extern std::shared_ptr<ButtonObject> btnObj3(new ButtonObject);
UIModel* pNewBtnSetClone;
Object2D* pNewbtn1;
Object2D* pNewbtn2;
Object2D* pNewbtn3;
bool IntroWorld::CreateModelType()
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

	/// 로고  --------------------------------------------------------------
	std::shared_ptr<ImageObject> obj1(new ImageObject);
	obj1->m_csName = L"ImageObject:LOGO";
	obj1->Init();
	obj1->SetRectDraw({ 0,0, 340,220 });
	obj1->SetPosition(Vector2(0, 0));
	obj1->m_pColorTex = m_LOGOTex;
	obj1->m_pMaskTex = nullptr;
	obj1->m_pVShader = pVShader;
	obj1->m_pPShader = pPShader;
	if (!obj1->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}
	obj1->SetCollisionType(CollisionType::Ignore, SelectType::Select_Ignore);
	I_UI.m_list.insert(std::make_pair(L"logo", obj1));

	/// 대화상자--------------------------------------------------------------
	std::shared_ptr<ButtonObject> btnDlg(new ButtonObject);
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

	/// 버튼1-게임시작---------------------------------------------------------------
	
	btnObj1->m_csName = L"TButtonObject:btnGameStart";
	btnObj1->m_index = 1;
	btnObj1->Init();
	btnObj1->m_rtOffset = { 0, 0, 0, 0 };
	btnObj1->SetRectDraw({ 0,0, 340,100 });//334,82//340,100
	btnObj1->SetPosition(Vector2(0,0));

	Texture* pTex1 = I_Texture.Load(L"../../DX2D/data/gamestart_nor.png");
	Sound* pSound1 = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/gamestart_hover.png");
	pSound1 = I_Sound.Load("../../data/Sound/FootStepSound.wav");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/gamestart_sel.png");
	pSound1 = I_Sound.Load("../../data/Sound/FootStepSound_2.wav");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	pTex1 = I_Texture.Load(L"../../DX2D/data/gamestart_nor.png");
	pSound1 = I_Sound.Load("../../data/Sound/PianoSound_00.mp3");
	btnObj1->m_pStatePlayList.emplace_back(pTex1, pSound1);

	if (!btnObj1->Create(m_pd3dDevice, m_pContext ,
		L"../../data/shader/DefaultUI.txt",
		L"../../DX2D/data/gamestart_nor.png"))
	{
		return false;
	}
	btnObj1->SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnGameStart", btnObj1));

	/// 버튼2-랭크---------------------------------------------------------------
	
	btnObj2->m_csName = L"TButtonObject:btnRank";
	btnObj1->m_index = 2;
	btnObj2->Init();
	btnObj2->m_rtOffset = { 0, 0, 0, 0 };
	btnObj2->SetRectDraw({ 0,0, 340,100 });//334,82//340,100
	btnObj2->SetPosition(Vector2(0, 0));

	Texture* pTex2 = I_Texture.Load(L"../../DX2D/data/hardmode_nor.png");
	Sound* pSound2 = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	btnObj2->m_pStatePlayList.emplace_back(pTex2, pSound2);

	pTex2 = I_Texture.Load(L"../../DX2D/data/hardmode_hover.png");
	pSound2 = I_Sound.Load("../../data/Sound/FootStepSound.wav");
	btnObj2->m_pStatePlayList.emplace_back(pTex2, pSound2);

	pTex2 = I_Texture.Load(L"../../DX2D/data/hardmode_sel.png");
	pSound2 = I_Sound.Load("../../data/Sound/FootStepSound_2.wav");
	btnObj2->m_pStatePlayList.emplace_back(pTex2, pSound2);

	pTex2 = I_Texture.Load(L"../../DX2D/data/hardmode_nor.png");
	pSound2 = I_Sound.Load("../../data/Sound/PianoSound_00.mp3");
	btnObj2->m_pStatePlayList.emplace_back(pTex2, pSound2);

	if (!btnObj2->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../DX2D/data/hardmode_nor.png"))
	{
		return false;
	}
	btnObj2->SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnRank", btnObj2));

	/// 버튼3-유저셋---------------------------------------------------------------
	
	btnObj3->m_csName = L"TButtonObject:btnUserSet";
	btnObj1->m_index = 3;
	btnObj3->Init();
	btnObj3->m_rtOffset = { 0, 0, 0, 0 };
	btnObj3->SetRectDraw({ 0,0, 340,100 });//334,82//340,100
	btnObj3->SetPosition(Vector2(0, 0));

	Texture* pTex3 = I_Texture.Load(L"../../DX2D/data/rule_nor.png");
	Sound* pSound3 = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	btnObj3->m_pStatePlayList.emplace_back(pTex3, pSound3);

	pTex3 = I_Texture.Load(L"../../DX2D/data/rule_hover.png");
	pSound3 = I_Sound.Load("../../data/Sound/FootStepSound.wav");
	btnObj3->m_pStatePlayList.emplace_back(pTex3, pSound3);

	pTex3 = I_Texture.Load(L"../../DX2D/data/rule_sel.png");
	pSound3 = I_Sound.Load("../../data/Sound/FootStepSound_2.wav");
	btnObj3->m_pStatePlayList.emplace_back(pTex3, pSound3);

	pTex3 = I_Texture.Load(L"../../DX2D/data/rule_nor.png");
	pSound3 = I_Sound.Load("../../data/Sound/PianoSound_00.mp3");
	btnObj3->m_pStatePlayList.emplace_back(pTex3, pSound3);

	if (!btnObj3->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../DX2D/data/rule_nor.png"))
	{
		return false;
	}
	btnObj3->SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
	I_UI.m_list.insert(std::make_pair(L"btnUserSet", btnObj3));

	//버튼 3개 합치기 모델--------------------------------------------------------------------------------------
	std::shared_ptr<UIModelComposed> pComposedObj1(new UIModelComposed);
	pComposedObj1->m_csName = L"TUIModelComposedBtn";

	//ButtonObject* pDlgWindow = (ButtonObject*)I_UI.GetPtr(L"btnDlg")->Clone();
	//pDlgWindow->m_pParent = nullptr;
	//pDlgWindow->m_rtOffset = { 50, 50, 50, 50 };
	//pDlgWindow->SetRectDraw({ 0,0, g_rtClient.right / 3,g_rtClient.bottom / 3 });
	//pDlgWindow->SetPosition(Vector2(400, 300));
	//pComposedObj->Add(pDlgWindow);

	UIModel* pBtn01 = I_UI.GetPtr(L"btnGameStart")->Clone();
	pBtn01->m_csName = L"btnGameStartClone_ComposedList";
	pBtn01->m_pParent = nullptr;
	pBtn01->SetRectDraw({ 700,150, 340,100 });
	pBtn01->SetPosition(Vector2(700, 150));
	//m_pNextWorld = &m_GameWorld;
	pComposedObj1->Add(pBtn01);

	UIModel* pBtn02 = I_UI.GetPtr(L"btnRank")->Clone();
	pBtn02->m_csName = L"btnRankClone_ComposedList";
	pBtn02->m_pParent = pBtn01;
	pBtn02->SetRectDraw({ 0,0, 340,100 });
	pBtn02->SetPosition(pBtn01->m_vPos + Vector2(0, 100));
	pComposedObj1->Add(pBtn02);

	UIModel* pBtn03 = I_UI.GetPtr(L"btnUserSet")->Clone();
	pBtn03->m_csName = L"btnUserSetClone_ComposedList";
	pBtn03->m_pParent = pBtn02;
	pBtn03->SetRectDraw({ 0,0, 340,100 });
	pBtn03->SetPosition(pBtn02->m_vPos + Vector2(0, 100));
	pComposedObj1->Add(pBtn03);

	I_UI.m_list.insert(std::make_pair(L"btnset", pComposedObj1));
	//----------------------------------------------------------------------------------------
	
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
	m_BGTex = I_Texture.Load(L"../../DX2D/data/bg.png");
	m_LOGOTex = I_Texture.Load(L"../../DX2D/data/logo.png");

	CreateModelType();

	Object2D* pNewBK =I_UI.GetPtr(L"bg")->Clone();
	pNewBK->m_csName = L"TImageObjectClock:bk";
	pNewBK->SetPosition(Vector2(500, 250));
	pNewBK->UpdateData();
	//pNewBK->m_bFadeIn = true;
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBK));

	Object2D* pNewLOGO = I_UI.GetPtr(L"logo")->Clone();
	pNewLOGO->m_csName = L"TImageObjectClock:logo";
	pNewLOGO->SetPosition(Vector2(300, 250));
	pNewLOGO->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewLOGO));
	//========================================================================================
	
	pNewbtn1 = I_UI.GetPtr(L"btnGameStart")->Clone();
	pNewbtn1->m_csName = L"gamestart";
	pNewbtn1->SetPosition(Vector2(700, 150));
	pNewbtn1->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewbtn1));

	pNewbtn2 = I_UI.GetPtr(L"btnRank")->Clone();
	pNewbtn2->m_csName = L"hardmode";
	pNewbtn2->SetPosition(Vector2(700, 250));
	pNewbtn2->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewbtn2));

	pNewbtn3 = I_UI.GetPtr(L"btnUserSet")->Clone();
	pNewbtn3->m_csName = L"rule";
	pNewbtn3->SetPosition(Vector2(700, 350));
	pNewbtn3->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewbtn3));

	return true;
}
bool IntroWorld::Frame()
{
	if (m_bLoadZone&&m_pNextWorld1 && m_pNextWorld2 && m_pNextWorld3 != nullptr)//m_bLoadZone
	{
		I_ObjectMgr.Release();
		m_pNextWorld1->Load(L"world.txt");
		m_pNextWorld2->Load(L"world.txt");
		m_pNextWorld3->Load(L"world.txt");
	}
	//m_pBackGroundMusic->Frame();
	if (World::m_pWorld->btngame == true)
	{
		World::m_pWorld = m_pNextWorld1;
		World::m_pWorld->btngame = false;
	}
	//else if (hardmode == true)
	//{
	//	World::m_pWorld = m_pNextWorld1;
	//	hardmode = true;
	//	//m_pNextWorld1->hardmode
	//	//World::m_pWorld->hardmode = false;
	//}
	else if (World::m_pWorld->btnrule == true)
	{
		I_ObjectMgr.Release();
		World::m_pWorld = m_pNextWorld3;
		World::m_pWorld->btnrule = false;
	}
	static int iIndex = 0;


	//m_ExplosionObj.Frame();
	World::Frame();
	return true;
}
bool IntroWorld::Render()
{
	World::Render();
	
	return true;
}
bool IntroWorld::Release()
{
	World::Release();
	
	return true;
}