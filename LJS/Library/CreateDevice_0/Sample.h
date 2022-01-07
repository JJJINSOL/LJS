#pragma once
#include "Core.h"
#include <d3d11.h>
//��Ŀ���� libname("d3d11.lib")�� library����(lib)�� �߰��϶�� �˷��ش�
#pragma comment	(lib, "d3d11.lib")
class Sample : public Core
{
public:
	ID3D11Device* m_pd3dDevice;	// ����̽� ��ü, ���ҽ��� ������ ���
	ID3D11DeviceContext* m_pImmediateContext; // �ٺ��̽� ���ؽ�Ʈ ��ü, ������ ���
	IDXGISwapChain* m_pSwapChain; // ����ü�� ��ü, ��ȯ�Ǵ� IDXGISwapChain �������̽� �ּ�
	ID3D11RenderTargetView* m_pRenderTargetView; // ���� ����Ÿ�� �� , ������ ���� �߿��� ������ �� ����� ����

	D3D_FEATURE_LEVEL m_FeatureLevel;	// Direct3D Ư������ �Ӽ���	
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;	// ����ü�� �Ӽ���
	D3D11_VIEWPORT m_ViewPort;	// �� ��Ʈ �Ӽ��� / ����Ʈ = ������ ���� �� ��ü ������ ��ġ�� ǥ��
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	Sample()
	{
		m_pd3dDevice = nullptr;
		m_pImmediateContext = nullptr;
		m_pSwapChain = nullptr;
		m_pRenderTargetView = nullptr;
	}

};

