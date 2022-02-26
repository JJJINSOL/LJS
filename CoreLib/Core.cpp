#include "Core.h"
#include "ObjectMgr.h"
bool Core::CoreInit()
{
	m_GameTimer.Init();
	Input::Get().Init();
	if (SUCCEEDED(InitDevice()))
	{
		I_Shader.Set(m_pd3dDevice);
		I_Texture.Set(m_pd3dDevice);

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
	Init();

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MinLOD = FLT_MAX;
	sd.MaxLOD = FLT_MIN;
	HRESULT hr = m_pd3dDevice->CreateSamplerState(&sd, &m_pSamplerState);

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
	return true;
}
bool Core::CoreFrame()
{
	m_GameTimer.Frame();
	Input::Get().Frame();
	I_ObjectMgr.Frame();
	Frame();
	m_dxWrite.Frame();
	return true;
}
bool Core::CoreRender()
{
	
	float color[4] = { 1, 0, 0, 1 };//배경색
	//백버퍼 지워준다 - (ClearRenderTargetView)화면을 깨끗이 지워준다는 의미
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);

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

	if (m_pSamplerState)m_pSamplerState->Release();
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
