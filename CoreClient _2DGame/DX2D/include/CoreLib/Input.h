#pragma once
#include "Std.h"
enum KeyState
{
	KEY_FREE = 0,//키 안눌림
	KEY_UP,//눌러졌던 키에서 손 떼는 순간
	KEY_PUSH,//눌러지는 순간
	KEY_HOLD,//눌려있는 상태
};
class Input
{
public:
	DWORD m_dwKeyState[256]; //키보드 상태 배열
	POINT m_ptMouse;
	DWORD m_dwMouseState[3];

	//클래스가 private되어 있기에 사용하기 위해서는 이 함수를 이용
	//static으로 해놨기에 딱 한개만 생성된당!
	static Input& Get()
	{
		static Input theSingle;
		return theSingle;
	}
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	DWORD   GetKey(DWORD dwKey);
private:
	Input();//입력은 여러개가 있을 필요가 없기에 다른 곳에서는 사용 못하게 하자!
public:
	virtual ~Input();
};

