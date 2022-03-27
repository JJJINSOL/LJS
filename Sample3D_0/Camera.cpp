#include "Camera.h"
bool Camera:: Init()
{
	m_matView.CreateViewLook(m_vCamera, m_vTarget, m_vUp);
	// Projection
	m_matProj.PerspectiveFovLH(1.0f, 
							   100.0f,
							   BASIS_PI * 0.25f,
							   (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}
bool Camera::Frame()
{
		/*if (TInput::Get().GetKey(VK_LEFT))
	{
		m_vCamera.z -= g_fSecPerFrame * 1.0f;
	}
	if (TInput::Get().GetKey(VK_RIGHT))
	{
		m_vCamera.z += g_fSecPerFrame * 1.0f;
	}
	if (TInput::Get().GetKey(VK_UP))
	{
		m_vCamera.y += g_fSecPerFrame * 1.0f;
	}
	if (TInput::Get().GetKey(VK_DOWN))
	{
		m_vCamera.y -= g_fSecPerFrame * 1.0f;
	}*/
	m_matView.CreateViewLook(m_vCamera, m_vTarget, m_vUp);
	return true;
}
Camera:: Camera()
{
	m_vCamera.x = 0.0f;
	m_vCamera.y = 9.0f;
	m_vCamera.z = -15.0f;
	m_vUp = Vector3(0, 1, 0);
}
Camera:: ~Camera()
{

}