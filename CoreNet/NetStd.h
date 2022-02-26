#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//윈속은 제일 위로 항상 올리자 아니면 에러 한가득
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#pragma comment (lib, "ws2_32.lib")
#pragma comment	(lib, "winmm.lib")
using namespace std;
#ifdef _DEBUG
#pragma comment	(lib, "CoreNet_d.lib")
#else
#pragma comment	(lib, "CoreNet_r.lib")
#endif