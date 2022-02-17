#include "Input.h"
DWORD Input::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}
bool Input::Init()
{
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}
bool Input::Frame()
{
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

}
Input::~Input()
{

}