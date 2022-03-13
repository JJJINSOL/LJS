#pragma once
#include "Object2D.h"
#include "ObjectMgr.h"
class PlayerObj2D : public Object2D
{
public:
	virtual void	HitOverlap(BaseObject* pObj, DWORD dwState);
	bool			Frame() override;
	//void  UpdateData() override
	//{
	//	m_rtCollision = Rect(m_vPos, m_fWidth, m_fHeight);
	//	SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
	//	I_ObjectMgr.AddCollisionExecute(this,
	//		std::bind(&PlayerObj2D::HitOverlap, this,
	//			std::placeholders::_1,
	//			std::placeholders::_2));
	//}
	PlayerObj2D();
	virtual ~PlayerObj2D();
};

