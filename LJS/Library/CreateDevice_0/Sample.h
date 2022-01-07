#pragma once
#include "Core.h"
#include <d3d11.h>
//링커에게 libname("d3d11.lib")의 library파일(lib)을 추가하라고 알려준다
#pragma comment	(lib, "d3d11.lib")
class Sample : public Core
{
public:
	ID3D11Device* m_pd3dDevice;	// 디바이스 객체, 리소스의 생성을 담당
	ID3D11DeviceContext* m_pImmediateContext; // 다비이스 컨텍스트 객체, 랜더링 담당
	IDXGISwapChain* m_pSwapChain; // 스왑체인 객체, 반환되는 IDXGISwapChain 인터페이스 주소
	ID3D11RenderTargetView* m_pRenderTargetView; // 메인 랜더타켓 뷰 , 생성된 버퍼 중에서 랜더링 될 대상을 지정

	D3D_FEATURE_LEVEL m_FeatureLevel;	// Direct3D 특성레벨 속성값	
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;	// 스왑체인 속성값
	D3D11_VIEWPORT m_ViewPort;	// 뷰 포트 속성값 / 뷰포트 = 일정한 범위 및 전체 영역을 장치에 표현
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

