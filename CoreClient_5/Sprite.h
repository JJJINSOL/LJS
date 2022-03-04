#pragma once
#include "Object2D.h"
class Sprite : public Object2D
{
public:
	float			   m_fAnimTime;
	std::vector<RECT>  m_rtArray;
public:
	void			SetRectSouceArray(RECT rt);
	virtual bool	Init() override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool	Release()override;
public:
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
};
//=================================================================
class SpriteMgr : public BaseMgr<Sprite, SpriteMgr>
{
	friend class Singleton<SpriteMgr>;
public:
	Sprite* Load(std::wstring filename) override;
private:
	SpriteMgr() {};
public:
	~SpriteMgr() {};
};
#define I_Sprite SpriteMgr::Get()
