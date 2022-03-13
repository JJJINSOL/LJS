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
	void  UpdateData() override
	{
		m_rtCollision = Rect(m_vPos, 30, 30);
		SetCollisionType(CollisionType::Overlap, SelectType::Select_Ignore);
		I_ObjectMgr.AddCollisionExecute(this,
			std::bind(&BaseObject::HitOverlap, this,
				std::placeholders::_1,
				std::placeholders::_2));
	}
public:
	ObjectNpc2D();
	virtual ~ObjectNpc2D();
};
