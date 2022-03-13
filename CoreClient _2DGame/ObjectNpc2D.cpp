#include "ObjectNpc2D.h"
//#include "GameWorld.h"

void ObjectNpc2D::HitOverlap(BaseObject* pObj, DWORD dwState)
{
	if (dwState == CollisionType::Overlap)
	{
		//Vector2 p;
		//p.y = -1.0f;

		//AddPosition(p);
	}
}
void ObjectNpc2D::HitSelect(BaseObject* pObj, DWORD dwState)
{
	if (m_dwSelectState & T_HOVER)
	{
		INT K = 0;
	}
	if (m_dwSelectState & T_FOCUS)
	{
		INT K = 0;
	}
	if (m_dwSelectState & T_ACTIVE)
	{
		INT K = 0;
	}
	if (m_dwSelectState & T_SELECTED)
	{
		m_bSelect = true;
		m_bDead = true;
	}
}
bool ObjectNpc2D::Frame()
{
	Vector2 p;

	p = m_vDirection * (m_fSpeed * g_fSecPerFrame);
	AddPosition(p);

	return true;
}

bool ObjectNpc2D:: Release()
{
	return true;
}
ObjectNpc2D::ObjectNpc2D()
{
	m_fSpeed = 50.0f + (rand() % 100); // ¼Ó·Â
	m_vDirection.y = 1.0f;
	m_dwCollisonType = CollisionType::Overlap;
	m_dwSelectType = SelectType::Select_Ignore;
	//I_ObjectMgr.AddCollisionExecute(this,
	//								std::bind(&BaseObject::HitOverlap, this,
	//								std::placeholders::_1,
	//								std::placeholders::_2));
}
ObjectNpc2D::~ObjectNpc2D()
{

}