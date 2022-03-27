#pragma once
#include "Std.h"
class Camera
{
public:
	Vector3	m_vCamera;
	Vector3	m_vTarget;
	Vector3	m_vUp;

	Matrix	m_matWorld;
	Matrix	m_matView;
	Matrix	m_matProj;
public:
	virtual bool	Init();
	virtual bool	Frame();
	Camera();
	virtual ~Camera();
};