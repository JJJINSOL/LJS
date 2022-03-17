#include "Object3D.h"
void Object3D::AddPosition(Vector3 vPos)
{
	// 현재위치
	m_vPos += vPos;
	//  10,10f    ~     w(50, 10)
	//
	//   h
	SetVertexData();
	SetIndexData();
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(
			m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	}
}
/// <summary>
/// m_rtDraw, m_rtCollision 갱신된다.
/// </summary>
/// <param name="vPos"></param>
void Object3D::SetPosition(Vector3 vPos)
{
	m_vPos = vPos;
	SetVertexData();
	SetIndexData();
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
	}
}
bool Object3D::SetVertexData()
{
	return true;
}
bool Object3D::SetIndexData()
{
	m_IndexList.clear();
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	return true;
}
void Object3D::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 0.5f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}
}
void Object3D::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
bool Object3D::Frame()
{
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	m_ConstantList.Color = m_vColor;
	m_ConstantList.Timer = Vector4(g_fGameTimer,
									0,
									0,
									1.0f);
	m_pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ConstantList, 0, 0);
	return true;
}
Object3D::Object3D()
{
	m_fAlpha = 1.0f;
	m_vColor = Vector4(1, 1, 1, 1);
}
Object3D::~Object3D()
{

}