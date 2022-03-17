#include "PlayerObj2D.h"
#include "Input.h"

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
	m_fSpeed = 600.0f; // 속력
}

PlayerObj2D::~PlayerObj2D()
{

}