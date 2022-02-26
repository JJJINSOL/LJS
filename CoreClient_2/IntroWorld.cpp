#include "IntroWorld.h"
#include "Input.h"
#include <string>
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
	Texture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	Shader* pVShader = I_Shader.CreateVertexShader(m_pd3dDevice, L"Shader.txt", "VS");
	Shader* pPShader = I_Shader.CreatePixelShader(m_pd3dDevice, L"Shader.txt", "PSAlphaBlend");
	ImageObject* obj = new ImageObject;
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(Vector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	obj->m_pColorTex = m_pColorTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;

	if (!obj->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}

	m_UIObj.insert(std::make_pair(L"bk", obj));

	ImageObject* btnObj = new ImageObject;
	btnObj->Init();
	btnObj->SetRectDraw({ 0,0, 334,82 });
	btnObj->SetPosition(Vector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	if (!btnObj->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../data/main_start_nor.png"))
	{
		return false;
	}
	m_UIObj.insert(std::make_pair(L"btnStart", btnObj));

	return true;
}
bool IntroWorld::Frame()
{
	m_pBackGroundMusic->Frame();
	static int iIndex = 0;
	//for (auto obj : m_UIObj)
	//{
	//	Object2D* pObj = obj.second;
	//	if (pObj != nullptr)
	//	{
	//		pObj->m_ConstantList.Color = Vector4(
	//			cosf(g_fGameTimer) * 0.5f + 0.5f,
	//			cosf(g_fGameTimer) * 0.5f + 0.5f,
	//			cosf(g_fGameTimer) * 0.5f + 0.5f, 1.0f);
	//		pObj->m_ConstantList.Timer = Vector4(
	//			g_fGameTimer,
	//			0,
	//			0,
	//			1.0f);
	//		m_pContext->UpdateSubresource(
	//			pObj->m_pConstantBuffer, 0, NULL, &pObj->m_ConstantList, 0, 0);
	//	}
	//}
	if (Input::Get().GetKey(VK_F2) == KEY_PUSH)
	{
		iIndex++;
		auto data = m_UIObj.find(L"bk");
		if (data != m_UIObj.end())
		{
			data->second->m_pColorTex = m_pChangeColorTex[iIndex];
		}
		if (iIndex >= 10)
		{
			iIndex = 0;
		}
	}
	/*
{
	auto data = m_UIObj.find(L"bk");
	if (data != m_UIObj.end())
	{
		data->second->m_pColorTex = m_pColorTex;
	}
}*/
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