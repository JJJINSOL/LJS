#include "Input.h"
POINT g_ptMouse;
DWORD Input::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}
bool Input::Init()
{
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}
T::TVector2 Input::GetDelta()
{
	T::TVector2 ret;
	m_fRadianY += (Input::Get().m_ptDeltaMouse.x / (float)g_rtClient.right) * BASIS_PI;
	m_fRadianX += (Input::Get().m_ptDeltaMouse.y / (float)g_rtClient.bottom) * BASIS_PI;
	return TVector2(m_fRadianX, m_fRadianY);
}
bool Input::Frame()
{
	POINT ptOffset = g_ptMouse;
	//ȭ��(��ũ��)��ǥ��
	GetCursorPos(&m_ptMouse);
	//Ŭ���̾�Ʈ(������)��ǥ��
	ScreenToClient(g_hwnd, &m_ptMouse);
	g_ptMouse = m_ptMouse;

	if (m_ptMouse.x < 0 || m_ptMouse.x > g_rtClient.right || m_ptMouse.y < 0 || m_ptMouse.y > g_rtClient.bottom)
	{
		m_bDrag = false;
	}

	// ���콺 ��ư VK_LBUTTON,  VK_RBUTTON, VK_MBUTTON,
	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = GetAsyncKeyState(iKey);//�񵿱�� �ش� Ű ���� �˷���!
		// 0000 0000 0000 0000 -> �Է�X
		// 1000 0000 0000 0000 -> ���� Ű�Է��� �ִٸ� �ֻ��� ���� ��Ʈ 1�̵�
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)//�������´� free�ε� ������ �����������ΰ��
			{
				m_dwKeyState[iKey] = KEY_PUSH;//Ű�� �������� ������
			}
			else
			{	
				//�������°� free�� �ƴϰ� ���� ������ ���¶�� ��� ������ �ִ� ��Ȳ
				m_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else
		{
			//���� �Է��� ����!
			if (m_dwKeyState[iKey] == KEY_PUSH || m_dwKeyState[iKey] == KEY_HOLD)
			{
				//�������� �Է��� �־����� �������� Ű���� ���� ���� ������
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				//������ ���ݵ� �Է¾���!
				m_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}
	m_dwMouseState[0] = m_dwKeyState[VK_LBUTTON];
	m_dwMouseState[1] = m_dwKeyState[VK_RBUTTON];
	m_dwMouseState[2] = m_dwKeyState[VK_MBUTTON];

	if (m_dwMouseState[0] == KEY_PUSH)
	{
		m_bDrag = true;
		m_ptMouseClick = m_ptMouse;
	}

	if (m_dwMouseState[0] == KEY_UP)
	{
		m_bDrag = false;
	}
	if (m_bDrag)
	{
		m_ptDeltaMouse.x = ptOffset.x - m_ptMouse.x;
		m_ptDeltaMouse.y = ptOffset.y - m_ptMouse.y;
	}
	return true;

	return true;
}
bool Input::Render()
{
	return true;
}
bool Input::Release() 
{
	return true;
}
Input::Input()
{
	m_ptDeltaMouse.x = 0.0f;
	m_ptDeltaMouse.y = 0.0f;
}
Input::~Input()
{

}