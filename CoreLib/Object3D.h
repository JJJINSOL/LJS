#pragma once
#include "DxObject.h"
class Object3D : public DxObject
{
public:
	Vector3			m_vPos;
	Vector3			m_vDirection;
	Vector4			m_vColor;
public:
	float  m_fAlpha = 0.0f;
	bool   m_bFadeIn = false;
	bool   m_bFadeOut = false;
	virtual void    FadeIn();
	virtual void    FadeOut();
public:
	virtual void		AddPosition(Vector3 vPos);
	virtual void		SetPosition(Vector3 vPos);
public:
	virtual bool		SetVertexData() override;
	virtual bool		SetIndexData() override;
	virtual bool		Frame() override;
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
		return true;
	};
	virtual void  UpdateData() {}
public:
	Object3D();
	virtual ~Object3D();
};

