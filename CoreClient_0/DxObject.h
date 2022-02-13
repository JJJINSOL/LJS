#pragma once
#include "Std.h"
#include <d3dcompiler.h>
#pragma comment	(lib, "d3dcompiler.lib")
struct SimpleVertex 
{
	Vector2 v;
	Vector2 t;
};
class DxObject
{
	float m_fSpeed;
	Vector2 m_vPos;

	std::vector<SimpleVertex> m_InitScreenList;
	std::vector<SimpleVertex> m_VertexList;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
public:
	void SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	bool Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext, Vector2 pos, float ww, float hh);

	// 화면좌표 위치를 중점으로 NDC 변환
	void Convert(Vector2 center, float w, float h, std::vector<SimpleVertex>& rectlist);

	//화면 좌표계를 NDC 변환
	void Convert(std::vector<SimpleVertex>& list, std::vector<SimpleVertex>& rectlist);

	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	DxObject()
	{
		m_fSpeed = 0.00001f;
	}
	~DxObject()
	{}
};

