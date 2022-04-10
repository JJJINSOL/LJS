#pragma once
#include "Core.h"
#include "PlaneObj.h"
#include "Camera.h"
#include "Map.h"
#include "SkyObj.h"
#include "BoxObj.h"
class Sample : public Core
{
	Camera		   m_Camera;
	PlaneObj       m_MapObj;
	PlaneObj       m_PlayerObj;
	PlaneObj       m_ObjB;
	SkyObj		   m_SkyObj;
public:
	virtual void	CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void	DeleteResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
public:
	Sample();
	virtual ~Sample();
};

