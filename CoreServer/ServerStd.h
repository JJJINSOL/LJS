#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <list>
#include <ws2tcpip.h> // inet_ntop
#pragma comment (lib, "ws2_32.lib")

#ifdef _DEBUG
#pragma comment	(lib, "CoreServer_d.lib")
#else
#pragma comment	(lib, "CoreServer_r.lib")
#endif

using namespace std;