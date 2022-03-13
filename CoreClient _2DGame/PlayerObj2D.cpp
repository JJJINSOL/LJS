#include "PlayerObj2D.h"
#include "Input.h"
//#include "GameWorld.h"
void PlayerObj2D::HitOverlap(BaseObject* pObj, DWORD dwState)
{
	if (dwState == CollisionType::Overlap)
	{
		//pObj->m_fHeight = 100;
		pObj->m_bDead = true;
		//AddPosition(p);
	}
}
bool PlayerObj2D::Frame()
{

	if (Input::Get().GetKey(VK_LEFT) == KEY_HOLD)
	{
		Vector2 pos;
		pos.x -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}

	if (Input::Get().GetKey(VK_RIGHT) == KEY_HOLD)
	{
		Vector2 pos;
		pos.x += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	return true;
}

PlayerObj2D::PlayerObj2D()
{
	m_fSpeed = 200.0f; // ¼Ó·Â
	m_dwCollisonType = CollisionType::Overlap;
	m_dwSelectType = SelectType::Select_Ignore;
	//I_ObjectMgr.AddCollisionExecute(this,
	//								std::bind(&BaseObject::HitOverlap, this,
	//								std::placeholders::_1,
	//								std::placeholders::_2));
	m_life = 3;
}

PlayerObj2D::~PlayerObj2D()
{

}