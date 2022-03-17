#pragma once
#include "Core.h"
#include "Object3D.h"
class PlaneObj : public Object3D
{
	Vector3				m_vCamera;
	Matrix				m_matWorld;
	Matrix				m_matView;
	Matrix				m_matProj;
public:
	PlaneObj()
	{
		m_vCamera.x = 0.0f;
		m_vCamera.y = 0.0f;
		m_vCamera.z = -5.0f;
	}
	virtual bool SetVertexData()
	{
		// Local Coord
		// Box
		//  0        1 
		//     원점
		//  2        3
		m_VertexList.resize(4);
		// -Z plane
		m_VertexList[0].p = Vector3(-1.0f, 1.0f, 3.5f);
		m_VertexList[0].n = Vector3(0.0f, 0.0f, -1.0f);
		m_VertexList[0].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[0].t = Vector2(0.0f, 0.0f);

		m_VertexList[1].p = Vector3(1.0f, 1.0f, 3.5f);
		m_VertexList[1].n = Vector3(0.0f, 0.0f, -1.0f);
		m_VertexList[1].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[1].t = Vector2(1.0f, 0.0f);


		m_VertexList[2].p = Vector3(-1.0f, -1.0f, 3.5f);
		m_VertexList[2].n = Vector3(0.0f, 0.0f, -1.0f);
		m_VertexList[2].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[2].t = Vector2(0.0f, 1.0f);


		m_VertexList[3].p = Vector3(1.0f, -1.0f, 3.5f);
		m_VertexList[3].n = Vector3(0.0f, 0.0f, -1.0f);
		m_VertexList[3].c = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_VertexList[3].t = Vector2(1.0f, 1.0f);
		return true;
	}
	virtual bool SetIndexData()
	{
		m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
		m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
		return true;
	}
	virtual bool Frame()
	{

		// world
		Matrix matRotate, matScale, matTrans;
		matRotate.ZRotate(g_fGameTimer);
		matScale.Scale(cos(g_fGameTimer) * 0.5f + 0.5f, 1.0f, 1.0f);
		matTrans.Translation(0,
			cos(g_fGameTimer) * 0.5f + 0.5f, 0);
		//m_matWorld = matRotate;
		// view
		if (Input::Get().GetKey(VK_LEFT))
		{
			m_vCamera.z -= g_fSecPerFrame * 1.0f;
		}
		if (Input::Get().GetKey(VK_RIGHT))
		{
			m_vCamera.z += g_fSecPerFrame * 1.0f;
		}
		if (Input::Get().GetKey(VK_UP))
		{
			m_vCamera.y += g_fSecPerFrame * 1.0f;
		}
		if (Input::Get().GetKey(VK_DOWN))
		{
			m_vCamera.y -= g_fSecPerFrame * 1.0f;
		}

		Vector3 vTarget(0, 0, 0);
		Vector3 vUp(0, 1, 0);
		m_matView.CreateViewLook(m_vCamera, vTarget, vUp);//뷰 행렬 계산(내적)
		// Projection
		m_matProj.PerspectiveFovLH(1.0f, 100.0f, BASIS_PI * 0.5f, 800.0f / 600.0f);//투영 행렬

		m_ConstantList.matWorld = m_matWorld.Transpose();
		m_ConstantList.matView = m_matView.Transpose();
		m_ConstantList.matProj = m_matProj.Transpose();

		Object3D::Frame();
		return true;
	}
};

class Sample : public Core
{
	PlaneObj       m_Obj;
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

