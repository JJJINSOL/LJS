#pragma once
#include "Window.h"
class Device : public Window
{
public:
	ID3D11Device* m_pd3dDevice;	// ����̽� ��ü
	// ����̽� ���ؽ�Ʈ = ȭ�� ��¿� �ʿ��� ��� ������ ���� ����ü, GDI�� ���� ����
	ID3D11DeviceContext* m_pImmediateContext;// �ٺ��̽� ���ؽ�Ʈ ��ü
	IDXGISwapChain* m_pSwapChain;	// ����ü�� ��ü
	ID3D11RenderTargetView* m_pRenderTargetView;// ���� ����Ÿ�� ��

	D3D_FEATURE_LEVEL       m_FeatureLevel;	// Direct3D Ư������ �Ӽ���	
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;	// ����ü�� �Ӽ���
	D3D11_VIEWPORT			m_ViewPort;	// �� ��Ʈ �Ӽ���
public:
	HRESULT InitDevice();
	virtual bool CreateDevice();
	virtual bool CreateRenderTargetView();
	virtual bool SetViewport();

	virtual bool CleanupDevice();
public:
	Device()
	{
		m_pd3dDevice = nullptr;
		m_pImmediateContext = nullptr;
		m_pSwapChain = nullptr;
		m_pRenderTargetView = nullptr;
	}
	virtual ~Device() {}
};
