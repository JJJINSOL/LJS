#include "UsersetWorld.h"
bool UsersetWorld::CreateModelType()
{
	/// ����̹���---------------------------------------------------------------
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
bool UsersetWorld::Init()
{
	World::Init();
	return true;
}
bool UsersetWorld::Load(std::wstring file)
{
	m_BGTex = I_Texture.Load(L"../../DX2D/data/bg.png");

	m_pBackGroundMusic = I_Sound.Load("../../DX2D/data/END.MP3");
	//m_pBackGroundMusic->m_pChannel->setMode(FMOD_LOOP_OFF);
	CreateModelType();

	//��� ����
	Object2D* pNewBK = I_UI.GetPtr(L"bg")->Clone();
	pNewBK->m_csName = L"TImageObjectClock:bk";
	pNewBK->SetPosition(Vector2(500, 250));
	pNewBK->UpdateData();
	m_UIObj.push_back(std::shared_ptr<Object2D>(pNewBK));
	return true;
}
bool UsersetWorld::Frame()
{
	//m_pBackGroundMusic->Frame();
	World::Frame();
	return true;
}
bool UsersetWorld::Render()
{
	World::Render();
	return true;
}
bool UsersetWorld::Release()
{
	World::Release();
	return true;
}