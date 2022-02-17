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
		SHORT sKey = GetAsyncKeyState(iKey);//비동기로 해당 키 상태 알려줘!
		// 0000 0000 0000 0000 -> 입력X
		// 1000 0000 0000 0000 -> 무언가 키입력이 있다면 최상위 단위 비트 1이됨
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)//이전상태는 free인데 지금은 눌러진상태인경우
			{
				m_dwKeyState[iKey] = KEY_PUSH;//키가 눌러지는 순간임
			}
			else
			{	
				//이전상태가 free가 아니고 지금 눌러진 상태라면 계속 누르고 있는 상황
				m_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else
		{
			//지금 입력이 없음!
			if (m_dwKeyState[iKey] == KEY_PUSH || m_dwKeyState[iKey] == KEY_HOLD)
			{
				//이전에는 입력이 있었으니 눌러졌던 키에서 손을 떼는 순간임
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				//이전도 지금도 입력없음!
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