#pragma once
#include "Core.h"
#include "TFbxObj.h"
#include "Map.h"
#include "Quadtree.h"
#include "QuadObject.h"
#include "DxRT.h"
class SampleMap : public Map
{
public:
	virtual float GetHeight(int index) override
	{
		return Map::GetHeight(index) * 0.1f;
	}
	virtual ~SampleMap() {}
};
class Sample : public Core
{
	SampleMap				m_MapObj;
	Quadtree				m_Quadtree;
	std::vector<Fbx>		m_FbxObj;
	QuadObject				m_QuadObj;
	Shader*					m_pShadowPShader = nullptr;
	Texture*				m_pLightTex;
	Texture*				m_pNormalMap;
	bool					m_bDepthShadow = true;
	ComPtr<ID3D11RasterizerState>  m_pRSSlopeScaledDepthBias;
public:
	Shader*		m_pProjShadowVShader = nullptr;
	Shader*		m_pProjShadowPShader = nullptr;
	DxRT		m_dxRT;
	TVector3	m_vLightPos;
	TVector3	m_vLightDir;
	TMatrix		m_matShadow;
	TMatrix		m_matViewLight;
	TMatrix		m_matProjLight;
	TMatrix		m_matTex;
	void		RenderDepthShadow(TMatrix* matView, TMatrix* matProj);
	void		RenderProjectionShadow(TMatrix* matView, TMatrix* matProj);
public:
	Shader*		m_pDepthShadowVShader = nullptr;
	Shader*		m_pDepthShadowPShader = nullptr;
public:
	virtual void	CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void	DeleteResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
public:
	bool	LoadMap();
	bool    LoadFbx();
public:
	void		RenderIntoBuffer(ID3D11DeviceContext* pContext);
	void		RenderMRT(ID3D11DeviceContext* pContext);
public:
	Sample();
	virtual ~Sample();
};

