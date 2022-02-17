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
	int			m_iCollisionID;//�Ŵ������� id����ؼ� ����ũ�� �� ������
	float		m_fSpeed;
	Vector2	m_vPos;
	Vector2	m_vDirection;
	float		m_fWidth;
	float		m_fHeight;
	Rect		m_rtCollision;//�����̰� �Ǹ� ������ �浹 ����
	DWORD		m_dwCollisonType;
	bool		m_bAlphaBlend = true;
public:
	//�浹ó���Լ�
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

	//�������̽� �ʿ�
	//�ؽ��Ŀ� ���õ� �������̽�
	//view = �������α׷����� shader�� ���޵Ǵ� ���ҽ��̴�
	ID3D11ShaderResourceView* m_pSRV0;//�ؽ��ĸ� GPU�� �����Ҷ��� �̰� �Ǿߵ�
	ID3D11Texture2D* m_pTexture0;//�ؽ��Ĵ� �����ε� ������ �ε��ϸ� �̰� �������

	ID3D11ShaderResourceView* m_pSRV1;
	ID3D11Texture2D* m_pTexture1;
	
	//����
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
