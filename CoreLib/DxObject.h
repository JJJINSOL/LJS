#pragma once
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "DxState.h"
#include "Collision.h"
struct SimpleVertex//����
{
	T::TVector2 v;
	T::TVector2 t;
};
struct Vertex
{
	T::TVector3 p; // ��ġ
	T::TVector3 n; // �븻(���� ����)
	T::TVector4 c; // �÷�
	T::TVector2 t; // �ؼ�(�ؽ�����ǥ/�ȼ�)

	Vertex(T::TVector3 p1,T::TVector3 n1,T::TVector4 c1,T::TVector2 t1)
	{
		p = p1;
		n = n1;
		c = c1;
		t = t1;
	}
	Vertex() {}
};
enum CollisionType
{
	Block = 0,
	Overlap,
	Ignore,
};
enum SelectType
{
	Select_Block = 0,
	Select_Overlap,
	Select_Ignore,
};
// 0001  - �⺻
// 0010  - ����
// 0100  - ����
// 1000  - ����ź
// 0111 = 7(�Ӽ�)
// 1111 = 15
enum SelectState
{
	T_DEFAULT = 0,  // Ŀ���� ���� ���� ��(T_FOCUS���¿��� �ٸ� ���� T_ACTIVE�ϸ� ��ȯ�ȴ�.)
	T_HOVER = 1,	// Ŀ���� ���� ���� ��
	T_FOCUS = 2,	// T_ACTIVE���¿��� ���� ��ư�� �ٸ� ������ ������ ��(���)
	T_ACTIVE = 4,	// ���콺 ���� ��ư ������ ���� ��
	T_SELECTED = 8, // T_ACTIVE ���¿��� ���ɹ�ư ������ ��
};
//====================================================================
class BaseObject
{
public:
	std::wstring m_csName;

	BaseObject* m_pParent = nullptr;
	bool		m_bDead;
	int			m_iCollisionID;//�Ŵ������� id����ؼ� ����ũ�� �� ������
	int			m_iSelectID;
	float		m_fSpeed;
	float		m_fWidth;
	float		m_fHeight;
	Rect		m_rtCollision;//�����̰� �Ǹ� ������ �浹 ����
	Box			m_BoxCollision;
	DWORD		m_dwCollisonType;
	DWORD		m_dwSelectType;
	DWORD		m_dwSelectState;
	DWORD		m_dwPreSelectState;
	bool		m_bSelect;
	bool		m_bAlphaBlend;
	
	virtual void HitOverlap(BaseObject* pObj, DWORD dwState);//�浹ó���Լ�
	virtual void HitSelect(BaseObject* pObj, DWORD dwState);

	virtual void SetCollisionType(DWORD dwCollisionType, DWORD dwSelectType)
	{
		m_dwCollisonType = dwCollisionType;
		m_dwSelectType = dwSelectType;
	}
public:

	BaseObject()
	{
		m_bDead = false;
		m_bSelect = false;
		m_bAlphaBlend = true;
		m_dwSelectState = T_DEFAULT;
		m_iCollisionID = -1;
		m_iSelectID = -1;
		m_dwCollisonType = CollisionType::Ignore;
		m_dwSelectType = SelectType::Select_Ignore;
	}
};
//===========================================================-====
struct Index
{
	DWORD _0;
	DWORD _1;
	DWORD _2;
};
struct ConstantData
{
	T::TMatrix  matWorld;
	T::TMatrix  matView;
	T::TMatrix  matProj;
	T::TMatrix  matNormal;
	T::TVector4 Color;
	T::TVector4 Timer;
};
//=================================================================
struct BoneWorld  //ĳ���� ����
{
	T::TMatrix  matBoneWorld[255];
};
//=================================================================
struct LightData  //����
{
	T::TVector4   vLightDir;
	T::TVector4   vLightPos;
	T::TVector4   vCameraDir;
	T::TVector4   vCameraPos;
	T::TMatrix	  matLight;      // w*v*p*t
};
//=================================================================
class DxObject : public BaseObject
{
public:
	Texture* m_pColorTex = nullptr;
	Texture* m_pMaskTex = nullptr;
	Shader*  m_pVShader = nullptr;
	Shader*  m_pPShader = nullptr;

	D3D11_TEXTURE2D_DESC m_TextureDesc;

public:
	std::vector<SimpleVertex> m_InitScreenList;
	std::vector<Vertex> m_VertexList;
	
	ID3D11Buffer* m_pVertexBuffer = nullptr;

	std::vector<DWORD> m_IndexList;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	ConstantData    m_ConstantList;
	ID3D11Buffer*	m_pConstantBuffer = nullptr;

	LightData		m_LightConstantList;
	ID3D11Buffer*	m_pLightConstantBuffer = nullptr;

	ID3D11InputLayout* m_pVertexLayout = nullptr;
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	void		 SetDevice(ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* m_pContext);
	
	virtual bool LoadTexture(const TCHAR* szColorFileName, const TCHAR* szMaskFileName);

	virtual bool SetVertexData();
	virtual bool SetIndexData();
	virtual bool SetConstantData();
	virtual bool Create(ID3D11Device* m_pd3dDevice,
						ID3D11DeviceContext* m_pContext,
						const TCHAR* szShaderFileName = nullptr,
						const TCHAR* szTextureFileName = nullptr,
						const TCHAR* szMaskFileName = nullptr);
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool CreateVertexShader(const TCHAR* szFile);
	virtual bool CreatePixelShader(const TCHAR* szFile);
	virtual bool CreateInputLayout();
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool PreRender();
	virtual bool Draw();
	virtual bool PostRender();
	virtual bool Release();
public:
	DxObject();
	~DxObject();
};
