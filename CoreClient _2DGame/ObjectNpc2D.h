#pragma once
#include "Object2D.h"
#include "ObjectMgr.h"
class ObjectNpc2D : public Object2D
{
public:
	bool Frame() override;
	//bool Render() override;
	bool Release() override;
	void HitOverlap(BaseObject* pObj, DWORD dwState);
	void HitSelect(BaseObject* pObj, DWORD dwState);
	//void  UpdateData() override
	//{
	//	m_rtCollision = Rect(m_vPos, m_fWidth, m_fHeight);
	//	SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
	//	I_ObjectMgr.AddCollisionExecute(this,
	//		std::bind(&ObjectNpc2D::HitOverlap, this,
	//			std::placeholders::_1,
	//			std::placeholders::_2));
	//}
public:
	ObjectNpc2D();
	virtual ~ObjectNpc2D();
};
