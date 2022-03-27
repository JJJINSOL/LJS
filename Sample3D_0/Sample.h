#pragma once
#include "Core.h"
#include "PlaneObj.h"
#include "Camera.h"
class Sample : public Core
{
	Camera		    m_Camera;
	PlaneObj       m_MapObj;
	PlaneObj       m_PlayerObj;
	PlaneObj       m_ObjB;
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

