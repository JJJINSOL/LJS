#include "UIObject.h"
bool UIObject::Frame()
{
	DxObject::Frame();
	return true;
}
bool UIObject::Render()
{
	DxObject::Render();
	return true;
}
//===========================================================
bool ImageObject::Init()
{
	m_bFadeIn = true;
	return true;
}
bool ImageObject::Frame()
{
	DxObject::Frame();
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	m_ConstantList.Color = Vector4(
		m_fAlpha,
		m_fAlpha,
		m_fAlpha, 1.0f);
	m_ConstantList.Timer = Vector4(
		g_fGameTimer,
		0,
		0,
		1.0f);
	m_pContext->UpdateSubresource(
		m_pConstantBuffer, 0, NULL, &m_ConstantList, 0, 0);
	return true;
}
bool ImageObject::Render()
{
	DxObject::Render();
	return true;
}
void ImageObject::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 0.5f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}

}
void ImageObject::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
//======================================================
bool ButtonObject::Frame()
{
	DxObject::Frame();
	return true;
}
bool ButtonObject::Render()
{
	DxObject::Render();
	return true;
}