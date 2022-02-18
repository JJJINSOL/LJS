#pragma once
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <list>
#include <d3d11.h>
#include <map>
#include <functional>
#include <iostream>
#include "Collision.h"
#pragma comment	(lib, "d3d11.lib")
//#pragma comment	(lib, "CoreLib")
#pragma comment	(lib, "ws2_32.lib")

#ifdef _DEBUG
#pragma comment	(lib, "CoreLib_d.lib")
#else
#pragma comment	(lib, "CoreLib_r.lib")
#endif

using namespace std;

extern RECT g_rtClient;
extern HWND g_hwnd;
extern float g_fSecPerFrame;
extern float g_fGameTimer;
extern POINT g_ptMouse;

template<class T>
class Singleton
{
public://T->자식
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

#define GAME_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){   Sample core;   
#define GAME_WIN(s,x,y) if (core.SetWinClass(hInstance) == FALSE) return 1;   if (core.SetWindow(L#s, x, y) == FALSE) return 1;   core.GameRun();    return 1;}

//int WINAPI wWinMain(HINSTANCE hInstance,
//					  HINSTANCE hPrevInstance,
//					  LPWSTR lpCmdLine,
//					  int nCmdShow)
//{
//	TWindow win;
// ------------------------------------------------------------------------
//	if (win.WinRegisterClass(hInstance) == FALSE) return 1;
//	if (win.SetWindow(L"Win Sample") == FALSE) return 1;
//	win.WinRun();
//	return 1;
//}

#define SIMPLE_WIN() if (core.SetWinClass(hInstance) == FALSE) return 1;   if (core.SetWindow() == FALSE) return 1;   core.GameRun();    return 1;}

//GAME_START, GAME_WIN 실행시키는 매크로
#define GAME_RUN(s,x,y) GAME_START; GAME_WIN(s,x,y);
//GAME_START, SIMPLE_WIN 실행시키는 매크로
#define RUN() GAME_START; SIMPLE_WIN();
