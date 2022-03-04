#include "EffectObj.h"
void EffectObj::HitSelect(BaseObject* pObj, DWORD dwState)
{
	int k = 0;
}
void EffectObj::Reset()
{
	m_bDead = false;
	m_fTimer = 0.0f;
	m_iCurrentIndex = 0;
}
bool EffectObj::Init()
{
	m_vColor = Vector4(1, 1, 1, 1.0f);
	return true;
}
bool EffectObj::Frame()
{
	if (m_bDead) return true;
	m_fChangeTime = m_pSprite->m_fAnimTime / m_pSprite->m_rtArray.size();
	m_fTimer += g_fSecPerFrame;
	if (m_fTimer >= m_fChangeTime)
	{
		m_iCurrentIndex++;
		if (m_iCurrentIndex >= m_pSprite->m_rtArray.size())
		{
			m_iCurrentIndex = 0;
			m_bDead = true;
		}
		m_fTimer -= m_fChangeTime;
		SetRectSouce(m_pSprite->m_rtArray[m_iCurrentIndex]);
		SetRectDraw({ 0,0,
			m_pSprite->m_rtArray[m_iCurrentIndex].right,
			m_pSprite->m_rtArray[m_iCurrentIndex].bottom });
	}
	m_ConstantList.Color = m_vColor;
	m_ConstantList.Timer = Vector4(g_fGameTimer, 0, 0, 1.0f);
	m_pContext->UpdateSubresource(
		m_pConstantBuffer, 0, NULL, &m_ConstantList, 0, 0);

	AddPosition({ 0,0 });
	return true;
}
bool EffectObj::Render()
{
	if (m_bDead) return true;
	DxObject::Render();
	return true;
}
bool EffectObj::Release()
{
	Object2D::Release();
	return true;
}