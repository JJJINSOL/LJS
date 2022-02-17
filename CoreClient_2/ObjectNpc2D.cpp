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
	//ȭ�� ������ ��������ϰ� ����
	//50�� ���� ������ ������Ʈ�� ������ ��� �����̱⿡ ���κ� �������
	if (m_vPos.x > g_rtClient.right - 50.0f)
	{
		//�ݴ�� ������ ó��
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_rtClient.right - 50.0f;//�̰� �Ⱥ��̸� �Դٸ� ���ٸ��Ѵ�/��迡 �� ���̷���
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
	m_fSpeed = 20.0f + (rand() % 300); // �ӷ�
	m_vDirection.x = (rand() % 2 == 0) ? 1.0f : -1.0f;
	m_vDirection.y = (rand() % 2 == 0) ? 1.0f : -1.0f;
}
ObjectNpc2D::~ObjectNpc2D()
{

}