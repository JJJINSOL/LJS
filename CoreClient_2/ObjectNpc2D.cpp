#include "ObjectNpc2D.h"
void ObjectNpc2D::HitOverlap(BaseObject* pObj, DWORD dwState)
{
	if (dwState == CollisionType::Overlap)
	{
		pObj->m_bAlphaBlend = !pObj->m_bAlphaBlend;
	}
}
bool ObjectNpc2D::Frame()
{
	Vector2 p;
	//화면 밖으로 벗어나지못하게 제어
	//50을 넣은 이유는 오브젝트의 기준이 가운데 정점이기에 옆부분 접힐까바
	if (m_vPos.x > g_rtClient.right - 50.0f)
	{
		//반대로 오도록 처리
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_rtClient.right - 50.0f;//이거 안붙이면 왔다리 갔다리한다/경계에 딱 붙이려고
	}
	if (m_vPos.x < g_rtClient.left + 50.0f)
	{
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_rtClient.left + 50.0f;
	}
	if (m_vPos.y > g_rtClient.bottom - 50.0f)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = g_rtClient.bottom - 50.0f;
	}
	if (m_vPos.y < g_rtClient.top + 50.0f)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = g_rtClient.top + 50.0f;
	}
	p = m_vDirection * (m_fSpeed * g_fSecPerFrame);
	AddPosition(p);
	return true;
}
ObjectNpc2D::ObjectNpc2D()
{
	m_fSpeed = 20.0f + (rand() % 300); // 속력
	m_vDirection.x = (rand() % 2 == 0) ? 1.0f : -1.0f;
	m_vDirection.y = (rand() % 2 == 0) ? 1.0f : -1.0f;
}
ObjectNpc2D::~ObjectNpc2D()
{

}