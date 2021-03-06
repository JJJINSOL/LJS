#include "Sample.h"
#include "ObjectMgr.h"
#include <strsafe.h>
#include "BoxObj.h"
void Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
bool Sample::LoadMap()
{
	m_MapObj.Init();
	m_MapObj.SetDevice(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_MapObj.CreateHeightMap(L"../../data/map/heightMap513.bmp");
	m_MapObj.CreateMap(m_MapObj.m_iNumCols, m_MapObj.m_iNumRows, 10.0f);
	if (!m_MapObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(),L"MapRT.hlsl",L"../../data/map/002.jpg"))
	{
		return false;
	}
	m_Quadtree.m_pCamera = m_pMainCamera;
	m_Quadtree.Build(&m_MapObj, 5);
	/*CreateMapObject();
	for (int iObj = 0; iObj < MAX_NUM_OBJECTS; iObj++)
	{
		m_Quadtree.AddObject(m_pObjList[iObj]);
	}*/
	return true;
}
bool Sample::LoadFbx()
{
	std::vector<std::wstring> listname;
	// Greystone.fbx  LOD ?޽? 5?? 
	listname.push_back(L"../../data/fbx/Greystone.fbx");
	listname.push_back(L"../../data/fbx/idle.fbx");
	//listname.push_back(L"../../data/fbx/Man.fbx");
	T::LoadAllPath(L"../../data/fbx/AdvancedVillagePack/Meshes", listname);

	// 0 ~ 60  idel
	// 61 ~91  walk;
	// 92 ~ 116	  run
	// 120 ~205 jump
	// 	205 ~289  attack
	//listname.push_back(L"../../data/fbx/ManSkin.fbx");
	//listname.push_back(L"../../data/fbx/ManIdelIdel.fbx");
	//listname.push_back(L"../../data/fbx/ManIdelWalk.fbx");
	//listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	//listname.push_back(L"../../data/fbx/MultiCameras.fbx");
	//listname.push_back(L"../../data/fbx/SM_Barrel.fbx");	
	//listname.push_back(L"../../data/fbx/st00sc00.fbx");
	//listname.push_back(L"../../data/fbx/SM_Tree_Var01.fbx");
	//listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");
	//listname.push_back(L"../../data/fbx/Turret_Deploy1/Turret_Deploy1.fbx");

	I_ObjectMgr.Set(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_FbxObj.resize(listname.size());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		Fbx* pFbx = &m_FbxObj[iObj];
		pFbx->Init();
		pFbx->m_iShadowID  = (iObj*2)+1;
		pFbx->m_vShadowColor = TVector4(pFbx->m_iShadowID/255.0f, 1,1,1);
		pFbx->m_pMainCamera = m_pMainCamera;
		pFbx->m_pd3dDevice = m_pd3dDevice.Get();
		pFbx->m_pContext = m_pImmediateContext.Get();
		pFbx->m_pMeshImp = I_ObjectMgr.Load(listname[iObj]);	
		pFbx->m_pVShader = I_Shader.CreateVertexShader(g_pd3dDevice, L"Character.hlsl", "VS");
		pFbx->m_pPShader = I_Shader.CreatePixelShader(g_pd3dDevice, L"Character.hlsl", "PSMRT");		

		pFbx->m_DrawList.resize(pFbx->m_pMeshImp->m_DrawList.size());
		int iRow = iObj / 10;
		int iCol = iObj / 10;
		int iOffRow = iObj % 10;
		int iOffCol = iObj % 10;
		T::TVector3 vPos = { -2000.0f + iOffCol * 300.0f,0, -2000.0f + iRow * 300.0f };
		vPos.y = m_MapObj.Map::GetHeight(vPos.x, vPos.z);
		pFbx->SetPosition(vPos);
		for (int iDraw = 0; iDraw < pFbx->m_pMeshImp->m_DrawList.size(); iDraw++)
		{
			pFbx->m_pMeshImp->m_DrawList[iDraw]->m_pContext = m_pImmediateContext.Get();
		}
	}

	m_FbxObj[0].m_pAnimImporter = m_FbxObj[1].m_pMeshImp;
	return true;
}
bool Sample::Init()
{
	HRESULT hr;
	// ?????Ͷ?????->bias ?? ????
	// ???? ?ٸ????? ???? ??ȭ?? ???? 
	// slope scaled depth bias(?????? ???? ????ġ) ?????ϴµ? ?????Ͷ????? ??????Ʈ ???? ????
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	//----------------------------------
	//???? ???鿡 ??????Ʈ?? ?ΰ? ?????? ?ɶ?(?????? ??ġ?? ??????Ʈ ?ΰ? ??????)
	//???? ???̰? ???? ?????? ?𸣴ϱ? ???̾ ???? ?־? ????
	rsDesc.DepthBias = 10000;				//z???? ???ؼ? ????
	rsDesc.DepthBiasClamp = 0.0f;			//???Ѱ?
	rsDesc.SlopeScaledDepthBias = 1.0f;		//?????? ???? ????ġ
	//--------------------------------------
	if (FAILED(hr = m_pd3dDevice->CreateRasterizerState(&rsDesc, m_pRSSlopeScaledDepthBias.GetAddressOf()))) return false;

	if (FAILED(m_QuadObj.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), L"Quad.hlsl")))
	{
		MessageBox(0, _T("m_QuadObj ????"), _T("Fatal error"), MB_OK);
		return 0;
	}
	//-------------------------------------------
	m_pShadowPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(), L"PlaneShadowCharacter.hlsl", "PSColor");

	LoadMap();
	LoadFbx();

	m_QuadObj.CreateTextures(m_pd3dDevice.Get(), m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height);
	m_QuadObj.SetBuffer(m_pd3dDevice.Get());
	m_QuadObj.ComputeKernel(9);

	m_pMainCamera->CreateViewMatrix(T::TVector3(-1000, 2000.0f, 0.0f), T::TVector3(-400.0f, 0.0f, 300.0f));
	m_pMainCamera->CreateProjMatrix(XM_PI * 0.25f,(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 30000.0f);
	
	m_pLightTex = I_Texture.Load(L"../../data/pung00.dds");
	m_pNormalMap = I_Texture.Load(L"../../data/NormalMap/tileADOT3.jpg");

	m_vLightPos = TVector3(5000, 8000, 100);
	T::D3DXVec3Normalize(&m_vLightDir, &m_vLightPos);
	
	if (m_bDepthShadow)
	{
		//???̸? ?ؽ??? ????
		m_dxRT.Create(m_pd3dDevice.Get(), 4096 * 4, 4096 * 4, DXGI_FORMAT_R24G8_TYPELESS);// DXGI_FORMAT_D32_FLOAT);
		m_pDepthShadowVShader = I_Shader.CreateVertexShader(m_pd3dDevice.Get(), L"DepthShadow.hlsl", "VS");
		m_pDepthShadowPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(), L"DepthShadow.hlsl", "PS");
	}
	else
	{
		m_dxRT.Create(m_pd3dDevice.Get(), 4096 * 4, 4096 * 4, DXGI_FORMAT_R24G8_TYPELESS);
		m_pProjShadowVShader = I_Shader.CreateVertexShader(m_pd3dDevice.Get(), L"ProjShadow.hlsl", "VS");
		m_pProjShadowPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(), L"ProjShadow.hlsl", "PS");
	}
	
	//?ؽ??? ??ȯ ????
	m_matTex = TMatrix(   0.5f, 0.0f, 0.0f, 0.0f
							, 0.0f, -0.5f, 0.0f, 0.0f
							, 0.0f, 0.0f, 1.0f, 0.0f
							, 0.5f, 0.5f, 0.0f, 1.0f);
	return true;
}
bool Sample::Frame()
{
	//????Ʈ ȸ??
	TMatrix matRotation;
	TVector3 vLight = m_vLightPos;
	D3DXMatrixRotationY(&matRotation, g_fGameTimer);// g_fGameTimer);
	D3DXVec3TransformCoord(&vLight, &vLight, &matRotation);
	D3DXVec3Normalize(&m_vLightDir, &vLight);

	m_QuadObj.Frame();
	m_MapObj.Frame();
	m_Quadtree.Update(m_pMainCamera);
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Frame();
	}
	//-------------------------------------------------------------
	TVector4 vClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (m_dxRT.Begin(m_pImmediateContext.Get(), vClearColor))
	{
		//?׸??ڸ? ????
		TVector3 vEye = vLight;
		TVector3 vLookat = { 0,0,0 };
		TVector3 vUp = TVector3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&m_matViewLight, &vEye, &vLookat, &vUp);
		//???????? ???? ???? - D3DXMatrixOrthoOffCenterLH
		D3DXMatrixOrthoOffCenterLH(&m_matProjLight,-6000 / 2, 6000.0f / 2, -6000.0f / 2, 6000.0f / 2, 0.0f, 20000.0f);
		if (m_bDepthShadow)
		{
			RenderDepthShadow(&m_matViewLight, &m_matProjLight);
		}
		else
		{
			RenderProjectionShadow(&m_matViewLight, &m_matProjLight);
		}
		m_dxRT.End(m_pImmediateContext.Get());
	}

	if (Input::Get().GetKey('8')==KEY_PUSH)
	{
		TextureMgr::SaveFile(m_pImmediateContext.Get(), m_dxRT.m_pTexture.Get(), L"proj.bmp");
	}
	return true;
}
void Sample::RenderDepthShadow(TMatrix* matView, TMatrix* matProj)
{
	//Ư?? ???? ????
	ApplyDSS(m_pImmediateContext.Get(), DxState::g_pDSSDepthEnable);
	ApplyRS(m_pImmediateContext.Get(), m_pRSSlopeScaledDepthBias.Get());// TDxState::g_pRSSlopeScaledDepthBias);

	m_MapObj.m_bAlphaBlend = false;
	m_MapObj.m_ConstantList.Color.z = 0.1f;
	m_MapObj.m_ConstantList.Color.w = 20000.0f;

	m_MapObj.SetMatrix(nullptr, matView, matProj);	
	m_MapObj.PreRender();
	m_MapObj.Draw();
	//m_pImmediateContext.Get()->VSSetShader(m_pDepthShadowVShader->m_pVertexShader, NULL, 0);
	m_pImmediateContext.Get()->PSSetShader(m_pDepthShadowPShader->m_pPixelShader, NULL, 0);
	m_MapObj.PostRender();

	ApplyBS(m_pImmediateContext.Get(), DxState::m_AlphaBlend);
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].m_vShadowColor.z = 0.1f;
		m_FbxObj[iObj].m_vShadowColor.w = 20000.0f;
		m_FbxObj[iObj].SetMatrix(nullptr, matView, matProj);
		m_FbxObj[iObj].RenderShadow(m_pDepthShadowPShader);
	}	
}
void Sample::RenderProjectionShadow(TMatrix* matView, TMatrix* matProj)
{
	ApplyDSS(m_pImmediateContext.Get(), DxState::g_pDSSDepthEnable);
	ApplyRS(m_pImmediateContext.Get(), DxState::g_pRSNoneCullSolid);
	ApplyBS(m_pImmediateContext.Get(), DxState::m_AlphaBlend);
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].SetMatrix(nullptr, matView, matProj);
		m_FbxObj[iObj].RenderShadow(m_pProjShadowPShader);
	}
}
bool Sample::Render()
{
	ApplyRS(m_pImmediateContext.Get(), DxState::g_pRSBackCullSolid);
	RenderIntoBuffer(m_pImmediateContext.Get());	
	ApplySS(m_pImmediateContext.Get(), DxState::m_pSSLinear);
	ApplySS(m_pImmediateContext.Get(), DxState::g_pSSClampLinear);
	m_QuadObj.SetMatrix(nullptr, nullptr, nullptr);
	m_QuadObj.Render();

//#ifdef _DEBUG
//	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
//	{
//		for (int iDraw = 0; iDraw < m_FbxObj[iObj].m_DrawList.size(); iDraw++)
//		{
//			g_pBoxDebug->SetMatrix(&m_FbxObj[iObj].m_pMeshImp->m_DrawList[iDraw]->m_matWorld,
//				&m_pMainCamera->m_matView,
//				&m_pMainCamera->m_matProj);
//			g_pBoxDebug->DrawDebugRender(&m_FbxObj[iObj].m_pMeshImp->m_DrawList[iDraw]->m_BoxCollision);
//		}
//	}
//#endif
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));

	ClearD3D11DeviceContext(m_pImmediateContext.Get());
	return true;
}
bool Sample::Release()
{
	m_dxRT.Release();
	m_QuadObj.Release();
	m_MapObj.Release();

	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Release();
	}
	return true;
}
void Sample::RenderIntoBuffer(ID3D11DeviceContext* pContext)
{
	//???? ???? Ÿ?? ????????
	ID3D11RenderTargetView* pOldRTV;
	ID3D11DepthStencilView* pOldDSV;
	pContext->OMGetRenderTargets(1, &pOldRTV, &pOldDSV);

	// ???? ?????? ?????? ?????? ????(??ī?̹ڽ?)?ϱ? ???ؼ? ???Ļ???.
	float color[4] ={	0, 0, 0, 0	};
	pContext->ClearRenderTargetView(m_QuadObj.m_pColorRTV.Get(), color);
	pContext->ClearRenderTargetView(m_QuadObj.m_pNormalDepthRTV.Get(), color);
	pContext->ClearDepthStencilView(m_QuadObj.m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);

	//???ο? ???? Ÿ?? ????
	ID3D11RenderTargetView* pViews[2];
	pViews[0] = m_QuadObj.m_pColorRTV.Get();
	pViews[1] = m_QuadObj.m_pNormalDepthRTV.Get();
	pContext->OMSetRenderTargets(2, pViews, m_QuadObj.m_pDepthStencilView.Get());
	//????
	RenderMRT(pContext);
	//???? ???? Ÿ?? ?????ϱ?
	pViews[0] = pOldRTV;
	pViews[1] = NULL;
	pContext->OMSetRenderTargets(2, pViews, pOldDSV);
	SAFE_RELEASE(pOldRTV);
	SAFE_RELEASE(pOldDSV);
}
void Sample::RenderMRT(ID3D11DeviceContext* pContext)
{
	pContext->OMSetDepthStencilState(DxState::g_pDSSDepthEnable, 0x00);
	ApplyBS(pContext, DxState::m_AlphaBlendDisable);

	//g_samComShadowMap HLSL?? ?ѱ???
	//PCF ?۾?
	ApplySS(pContext, DxState::g_pSSShadowMap, 2);

	m_MapObj.m_bAlphaBlend = false;
	m_MapObj.SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_MapObj.m_LightConstantList.matLight = m_matViewLight * m_matProjLight *m_matTex;
	T::D3DXMatrixTranspose(&m_MapObj.m_LightConstantList.matLight,&m_MapObj.m_LightConstantList.matLight);

	m_pImmediateContext->PSSetSamplers(1, 1, &DxState::g_pSSClampLinear);
	m_Quadtree.PreRender();	
	pContext->PSSetShaderResources(1, 1,m_dxRT.m_pDsvSRV.GetAddressOf());
	ApplySS(pContext, DxState::g_pSSShadowMap, 2);
	m_Quadtree.PostRender();

	if (m_pLightTex)	pContext->PSSetShaderResources(1, 1, m_pLightTex->m_pSRV.GetAddressOf());
	if (m_pNormalMap)	pContext->PSSetShaderResources(4, 1, m_pNormalMap->m_pSRV.GetAddressOf());
	if (m_dxRT.m_pSRV)	pContext->PSSetShaderResources(5, 1, m_dxRT.m_pSRV.GetAddressOf());

	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].m_LightConstantList.matLight = m_matViewLight * m_matProjLight * m_matTex;
		T::D3DXMatrixTranspose(&m_FbxObj[iObj].m_LightConstantList.matLight, &m_FbxObj[iObj].m_LightConstantList.matLight);
		m_FbxObj[iObj].SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_FbxObj[iObj].Render();
	}
	ClearD3D11DeviceContext(pContext);
}
Sample::Sample()
{

}
Sample::~Sample()
{}


RUN();