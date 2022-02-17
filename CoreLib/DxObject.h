#pragma once
#include "Std.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#pragma comment	(lib, "d3dcompiler.lib")
struct SimpleVertex
{
	Vector2 v;
	Vector2 t;
};
enum CollisionType
{
	Block = 0,
	Overlap,
	Ignore,
};
class BaseObject
{
public:
	int			m_iCollisionID;//매니저에서 id등록해서 유니크한 것 만들어낸것
	float		m_fSpeed;
	Vector2	m_vPos;
	Vector2	m_vDirection;
	float		m_fWidth;
	float		m_fHeight;
	Rect		m_rtCollision;//움직이게 되면 무조건 충돌 갱신
	DWORD		m_dwCollisonType;
	bool		m_bAlphaBlend = true;
public:
	//충돌처리함수
	virtual void	HitOverlap(BaseObject* pObj, DWORD dwState);
public:

	BaseObject()
	{
		m_iCollisionID = -1;
		m_vDirection.x = 0.0f;
		m_vDirection.y = 0.0f;
		m_dwCollisonType = Overlap;
	}
};
class DxObject : public BaseObject
{
public:

	//인터페이스 필요
	//텍스쳐와 관련된 인터페이스
	//view = 응용프로그램에서 shader로 전달되는 리소스이다
	ID3D11ShaderResourceView* m_pSRV0;//텍스쳐를 GPU에 전달할때는 이게 되야됨
	ID3D11Texture2D* m_pTexture0;//텍스쳐는 파일인데 파일을 로드하면 이게 만들어짐

	ID3D11ShaderResourceView* m_pSRV1;
	ID3D11Texture2D* m_pTexture1;
	
	//투명도
	ID3D11BlendState* m_AlphaBlend;
	ID3D11BlendState* m_AlphaBlendDisable;

	D3D11_TEXTURE2D_DESC m_TextureDesc;
public:
	float m_fSpeed;
	Vector2	m_vPos;
	float m_fWidth;
	float m_fHeight;
public:
	std::vector<SimpleVertex> m_InitScreenList;
	std::vector<SimpleVertex> m_VertexList;
	
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;

	ID3DBlob* m_pVSCodeResult = nullptr;
	ID3DBlob* m_pErrorMsgs = nullptr;
	ID3DBlob* m_pPSCodeResult = nullptr;
public:
	void    SetDevice(ID3D11Device* m_pd3dDevice,
		ID3D11DeviceContext* m_pContext);
	virtual bool    LoadTexture(const TCHAR* szColorFileName,
								const TCHAR* szMaskFileName);
	virtual bool    SetVertexData();
	virtual bool	Create(ID3D11Device* m_pd3dDevice,
							ID3D11DeviceContext* m_pContext,
							const TCHAR* szTextureFileName = nullptr,
							const TCHAR* szMaskFileName = nullptr);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	DxObject();
	~DxObject();
};
