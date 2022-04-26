#pragma once
#include "Std.h"
enum KeyState
{
	KEY_FREE = 0,	//Ű �ȴ���
	KEY_UP,			//�������� Ű���� �� ���� ����
	KEY_PUSH,		//�������� ����
	KEY_HOLD,		//�����ִ� ����
};
class Input
{
public:
	float m_fRadianX = 0.0f;
	float m_fRadianY = 0.0f;
	bool  m_bDrag = false;

	DWORD m_dwKeyState[256]; //Ű���� ���� �迭
	
	POINT m_ptMouse;
	POINT m_ptMouseClick;
	POINT m_ptDeltaMouse;
	
	DWORD m_dwMouseState[3];

	//Ŭ������ private�Ǿ� �ֱ⿡ ����ϱ� ���ؼ��� �� �Լ��� �̿�
	//static���� �س��⿡ �� �Ѱ��� �����ȴ�!
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

	T::TVector2 GetDelta();

private:
	Input();//�Է��� �������� ���� �ʿ䰡 ���⿡ �ٸ� �������� ��� ���ϰ� ����!
public:
	virtual ~Input();
};

