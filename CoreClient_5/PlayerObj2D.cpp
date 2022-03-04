#include "PlayerObj2D.h"
#include "Input.h"
void PlayerObj2D::HitOverlap(BaseObject* pObj, DWORD dwState)
{
	if (dwState == CollisionType::Overlap)
	{
		//pObj->m_bAlphaBlend = false;
	}
}
bool PlayerObj2D::Frame()
{
	if (Input::Get().GetKey('W') == KEY_HOLD)
	{
		Vector2 pos;
		// 시간의 동기화
		pos.y -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (Input::Get().GetKey('A') == KEY_HOLD)
	{
		Vector2 pos;
		pos.x -= m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (Input::Get().GetKey('S') == KEY_HOLD)
	{
		Vector2 pos;
		pos.y += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	if (Input::Get().GetKey('D') == KEY_HOLD)
	{
		Vector2 pos;
		pos.x += m_fSpeed * g_fSecPerFrame;
		AddPosition(pos);
	}
	return true;
}

PlayerObj2D::PlayerObj2D()
{
	m_fSpeed = 300.0f; // 속력
	m_dwCollisonType = CollisionType::Overlap;
	m_dwSelectType = SelectType::Select_Overlap;
}

PlayerObj2D::~PlayerObj2D()
{

}