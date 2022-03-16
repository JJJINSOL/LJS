#pragma once
#include "ObjectMgr.h"
#include "UIModelMgr.h"

class UIObject : public UIModel
{
public:
	UIModel* Clone()
	{
		UIModel* pCopy = new UIObject(*this);
		pCopy->CreateVertexBuffer();
		pCopy->CreateIndexBuffer();
		pCopy->CreateConstantBuffer();
		pCopy->CreateInputLayout();
		return pCopy;
	};
	void  UpdateData() override
	{
		m_rtCollision = Rect(m_vPos, m_fWidth, m_fHeight);
		SetCollisionType(CollisionType::Ignore, SelectType::Select_Overlap);
		I_ObjectMgr.AddCollisionExecute(this,
			std::bind(&BaseObject::HitOverlap, this,
				std::placeholders::_1,
				std::placeholders::_2));
		I_ObjectMgr.AddSelectExecute(this,
			std::bind(&BaseObject::HitSelect, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}

public:
	bool	Frame() override;
	bool	Render()override;
	bool    SetVertexData()override;
	bool    SetIndexData()override;
	void    HitSelect(BaseObject* pObj, DWORD dwState) override
	{
		int k = 0;
	}
	UIObject()
	{
		m_rtOffsetTex.left = 0;
		m_rtOffsetTex.top = 0;
		m_rtOffsetTex.right = 1;
		m_rtOffsetTex.bottom = 1;
	}
};
//=========================================================================
class ImageObject : public UIObject
{
	UIModel* Clone()
	{
		UIModel* pCopy = new ImageObject(*this);
		pCopy->CreateVertexBuffer();
		pCopy->CreateIndexBuffer();
		pCopy->CreateConstantBuffer();
		pCopy->CreateInputLayout();
		return pCopy;
	};
public:
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
	void    HitSelect(BaseObject* pObj, DWORD dwState) override;
};
//========================================================================
class ButtonObject : public UIObject
{
public:
	UIModel* Clone()
	{
		UIModel* pCopy = new ButtonObject(*this);
		pCopy->CreateVertexBuffer();
		pCopy->CreateIndexBuffer();
		pCopy->CreateConstantBuffer();
		pCopy->CreateInputLayout();
		return pCopy;
	};
	//TButtonObject(const TButtonObject& copy)
	//{
	//	TDxObject::CreateVertexBuffer();
	//}
	virtual void	HitSelect(BaseObject* pObj, DWORD dwState) override;
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
public:
	ButtonObject() {}
	virtual ~ButtonObject() {}
};
//========================================================================
class ListCtrlObject : public UIModelComposed
{
public:
	UIModel* Clone()
	{
		UIModelComposed* pModel = new ListCtrlObject;
		std::list< UIModel*>::iterator iter;
		for (iter = m_Components.begin(); iter != m_Components.end();
			iter++)
		{
			pModel->Add((*iter)->Clone());
		}
		return pModel;
	};
	bool Create(int xCount, int yCount);
	void UpdateData() override
	{
		m_rtCollision = Rect(m_vPos, m_fWidth, m_fHeight);
		SetCollisionType(CollisionType::Ignore,SelectType::Select_Overlap);

		I_ObjectMgr.AddCollisionExecute(this,
										std::bind(&BaseObject::HitOverlap, this,
										std::placeholders::_1,
										std::placeholders::_2));
		I_ObjectMgr.AddSelectExecute(this,
									 std::bind(&BaseObject::HitSelect, this,
								     std::placeholders::_1,
									 std::placeholders::_2));
	}
	virtual void HitSelect(BaseObject* pObj, DWORD dwState) override;
public:
	ListCtrlObject() {}
	virtual ~ListCtrlObject() {}
};
