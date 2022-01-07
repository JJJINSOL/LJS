#pragma once
#include "NetWork.h"
//+1을 해주는 이유는 WM_USER가 0x400이니까 이 값까지가 시스템이 이용한다. 그러니 다음 값인 +1값부터 맘대로 사용한다는 의미
//WM_USER - 시스템에서 사용자가 window message 로 사용할 수 있게 예약해둔 공간!
#define NETWORK_MSG WM_USER+1
class AsyncSelect : public NetWork
{
public:
	bool m_connect = false;
	bool Connect(HWND hwnd, int protocol, int port, const char* ip);
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

