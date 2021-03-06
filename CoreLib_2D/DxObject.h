#pragma once
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "DxState.h"
#include "Collision.h"
struct SimpleVertex//정점
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
enum SelectType
{
	Select_Block = 0,
	Select_Overlap,
	Select_Ignore,
};
// 0001  - 기본
// 0010  - 권총
// 0100  - 장총
// 1000  - 수류탄
// 0111 = 7(속성)
// 1111 = 15
enum SelectState
{
	T_DEFAULT = 0,  // 커서가 위에 없을 때(T_FOCUS상태에서 다른 곳을 T_ACTIVE하면 전환된다.)
	T_HOVER = 1,	// 커서가 위에 있을 때
	T_FOCUS = 2,	// T_ACTIVE상태에서 왼쪽 버튼을 다른 곳에서 놓았을 때(취소)
	T_ACTIVE = 4,	// 마우스 왼쪽 버튼 누르고 있을 때
	T_SELECTED = 8, // T_ACTIVE 상태에서 왼쪼버튼 놓았을 때
};
//====================================================================
class BaseObject
{
public:
	std::wstring m_csName;

	BaseObject* m_pParent = nullptr;
	bool		m_bDead;
	int			m_iCollisionID;//매니저에서 id등록해서 유니크한 것 만들어낸것
	int			m_iSelectID;
	float		m_fSpeed;
	Vector2		m_vPos;
	Vector2		m_vDirection;//방향
	float		m_fWidth;
	float		m_fHeight;
	Rect		m_rtCollision;//움직이게 되면 무조건 충돌 갱신
	DWORD		m_dwCollisonType;
	DWORD		m_dwSelectType;
	DWORD		m_dwSelectState;
	DWORD		m_dwPreSelectState;
	bool		m_bSelect;
	bool		m_bAlphaBlend;
	
	virtual void HitOverlap(BaseObject* pObj, DWORD dwState);//충돌처리함수
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
		m_vDirection.x = 0.0f;
		m_vDirection.y = 0.0f;
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
struct ConstantData//지속적인
{
	Vector4 Color;
	Vector4 Timer;
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
	std::vector<SimpleVertex> m_VertexList;
	
	ID3D11Buffer* m_pVertexBuffer = nullptr;

	std::vector<DWORD> m_IndexList;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	ConstantData    m_ConstantList;
	ID3D11Buffer* m_pConstantBuffer = nullptr;

	ID3D11InputLayout* m_pVertexLayout = nullptr;
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	void SetDevice(ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* m_pContext);
	
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
	virtual bool Release();
public:
	DxObject();
	~DxObject();
};
