#include "Core.h"
#include "ObjectMgr.h"
#include "SoundMgr.h"

BoxObj* g_pBoxDebug = nullptr;

bool Core::CoreInit()
{
	m_GameTimer.Init();
	Input::Get().Init();
	if (SUCCEEDED(InitDevice()))
	{
		I_Shader.Set(m_pd3dDevice.Get());
		I_Texture.Set(m_pd3dDevice.Get());
		DxState::SetState(m_pd3dDevice.Get());
		if (m_dxWrite.Init())
		{
			IDXGISurface1* pSurface = nullptr;
			HRESULT hr = m_pSwapChain->GetBuffer(0,
												__uuidof(IDXGISurface1),
												(void**)&pSurface);
			if (SUCCEEDED(hr))
			{
				m_dxWrite.SetRenderTarget(pSurface);
			}
			if (pSurface) pSurface->Release();
		}
	}

	m_DefaultCamera.Init();
	m_DefaultCamera.CreateViewMatrix(T::TVector3(0, 500.0f, -100.0f),T::TVector3(0, 0.0f, 0));
	m_DefaultCamera.CreateProjMatrix(XM_PI * 0.25f,(float)g_rtClient.right / (float)g_rtClient.bottom, 0.1f, 5000.0f);
	
	m_DefaultCamera.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	
	m_DefaultCamera.m_pVShader = I_Shader.CreateVertexShader(m_pd3dDevice.Get(), L"../../data/shader/Box.hlsl", "VSColor");;
	m_DefaultCamera.m_pPShader = I_Shader.CreatePixelShader(m_pd3dDevice.Get(), L"../../data/shader/Box.hlsl", "PSColor");;
	
	m_DefaultCamera.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_DefaultCamera.Create(m_pd3dDevice.Get(), m_pImmediateContext.Get()))
	{
		return false;
	}

	//메인 카메라에 디폴트 카메라 넣어줌 다른 카메라로 전환하고 싶으면 메인카메라에 넣어주기
	m_pMainCamera = &m_DefaultCamera;

	m_SkyObj.Init();
	m_SkyObj.SetPosition(T::TVector3(0.0f, 0.0f, 0.0f));

	if (!m_SkyObj.Create(m_pd3dDevice.Get(),
		m_pImmediateContext.Get(),
		L"../../data/shader/sky.hlsl",
		L"../../data/sky/skybox02.dds"))
	{
		return false;
	}

	DrawDebugInit(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	g_pBoxDebug = &m_BoxDebug;

	Init();

	return true;
}
bool Core::GameRun()
{
	CoreInit();
	while (WinRun())
	{
		CoreFrame();
		CoreRender();
	}
	CoreRelease();

	//MemoryReporting();

	return true;
}
bool Core::CoreFrame()
{
	if (Input::Get().GetKey('V') == KEY_PUSH)
	{
		m_bWireFrame = !m_bWireFrame;
	}
	m_GameTimer.Frame();
	Input::Get().Frame();
	m_pMainCamera->Frame();
	m_SkyObj.Frame();
	I_ObjectMgr.Frame();
	I_Sound.Frame();
	Frame();
	m_dxWrite.Frame();
	return true;
}
bool Core::CoreRender()
{
	//float color[4] = { 0.1543f, 0.23421f, 0.4323f,1.0f };
	float color[4] = { 1, 1, 1, 1 };//배경색
	//백버퍼 지워준다 - (ClearRenderTargetView)화면을 깨끗이 지워준다는 의미
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);

	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(),D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	m_pImmediateContext->OMSetRenderTargets(1,m_pRenderTargetView.GetAddressOf(),m_pDepthStencilView.Get());

	m_pImmediateContext->PSSetSamplers(0, 1, &DxState::m_pSSLinear);
	m_pImmediateContext->PSSetSamplers(1, 1, &DxState::m_pSSPoint);

	m_pImmediateContext->OMSetDepthStencilState(DxState::g_pDSSDepthEnable, 0x00);
	if (m_bWireFrame)
	{
		m_pImmediateContext->RSSetState(DxState::g_pRSBackCullWireFrame);
	}
	else
	{
		m_pImmediateContext->RSSetState(DxState::g_pRSBackCullSolid);
	}

	//------------------------------------------------------------------
	m_SkyObj.m_matView = m_pMainCamera->m_matView;
	m_SkyObj.m_matView._41 = 0;
	m_SkyObj.m_matView._42 = 0;
	m_SkyObj.m_matView._43 = 0;
	T::TMatrix matRotation, matScale;
	/*T::D3DXMatrixScaling(&matScale, 3000.0f, 3000.0f, 3000.0f);
	T::D3DXMatrixRotationY(&matRotation, g_fGameTimer * 0.00f);
	m_SkyObj.m_matWorld = matScale * matRotation;*/
	m_SkyObj.SetMatrix(NULL, &m_SkyObj.m_matView, &m_pMainCamera->m_matProj);
	m_pImmediateContext->RSSetState(DxState::g_pRSNoneCullSolid);
	//m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::m_pSSLinear);
	//m_pImmediateContext->PSSetSamplers(1, 1, &TDxState::m_pSSPoint);
	m_SkyObj.Render();
	m_pImmediateContext->RSSetState(DxState::g_pRSBackCullSolid);
	// -----------------------------------------------------------------
	// 백버퍼에 랜더링 한다.
	Render();

	m_GameTimer.Render();
	Input::Get().Render();

	m_dxWrite.Render();
	m_pSwapChain->Present(0, 0);
	return true;
}
bool Core::CoreRelease()
{
	Release();
	m_BoxDebug.Release();
	m_SkyObj.Release();
	m_DefaultCamera.Release();
	DxState::Release();
	m_dxWrite.Release();
	m_GameTimer.Release();
	Input::Get().Release();
	CleanupDevice();
	return true;
}
void Core::ResizeDevice(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == nullptr) return;
	DeleteResizeDevice(iWidth, iHeight);

	m_dxWrite.DeleteDeviceResize();

	Device::ResizeDevice(iWidth, iHeight);

	IDXGISurface1* pSurface = nullptr;
	HRESULT hr = m_pSwapChain->GetBuffer(0,
										__uuidof(IDXGISurface1),
										(void**)&pSurface);
	if (SUCCEEDED(hr))
	{
		m_dxWrite.SetRenderTarget(pSurface);
	}
	if (pSurface) pSurface->Release();

	CreateResizeDevice(iWidth, iHeight);
}
void Core::DrawDebugInit(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_BoxDebug.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_BoxDebug.m_pVShader = I_Shader.CreateVertexShader(pd3dDevice, L"Box.hlsl", "VSColor");
	m_BoxDebug.m_pPShader = I_Shader.CreatePixelShader(	pd3dDevice, L"Box.hlsl", "PSColor");
	m_BoxDebug.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_BoxDebug.Create(pd3dDevice, pContext))
	{
		return;
	}
}