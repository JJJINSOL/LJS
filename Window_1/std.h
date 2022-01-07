#pragma once
#include <windows.h>
#include <vector>
#include <list>
using namespace std;

#define GAME_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){   Sample core;   
#define GAME_WIN(s,x,y) if (core.WinRegisterClass(hInstance) == FALSE) return 1;   if (core.SetWindow(L#s, x, y) == FALSE) return 1;   core.GameRun();    return 1;}

//int WINAPI wWinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPWSTR    lpCmdLine,
//	int       nCmdShow)
//{
//	TWindow win;
// ------------------------------------------------------------------------
//	if (win.WinRegisterClass(hInstance) == FALSE) return 1;
//	if (win.SetWindow(L"Win Sample") == FALSE) return 1;
//	win.WinRun();
//	return 1;
//}

#define SIMPLE_WIN() if (core.WinRegisterClass(hInstance) == FALSE) return 1;   if (core.SetWindow() == FALSE) return 1;   core.GameRun();    return 1;}

//GAME_START, GAME_WIN 실행시키는 매크로
#define GAME_RUN(s,x,y) GAME_START; GAME_WIN(s,x,y);
//GAME_START, SIMPLE_WIN 실행시키는 매크로
#define SIMPLE_RUN() GAME_START; SIMPLE_WIN();
