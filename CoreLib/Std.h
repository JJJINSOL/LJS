#pragma once
#include <winsock2.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgidebug.h>
#include <tchar.h>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <atlconv.h> // A2W
#include "Collision.h"
#include <wrl.h> 
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include "TMath.h"

class BoxObj;

using namespace Microsoft::WRL;

#pragma comment	(lib, "d3d11.lib")
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dxgi.lib" )

#ifdef _DEBUG
#pragma comment	(lib, "CoreLib_d.lib")
#else
#pragma comment	(lib, "CoreLib_r.lib")
#endif

#pragma comment	(lib, "ws2_32.lib")

using namespace std;

#define BASIS_EPSILON		((FLOAT)  0.001f)
#define BASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (BASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / BASIS_PI))
#define MAKECOLOR_ARGB(a, r, g, b)	(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)


#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

extern RECT			 g_rtClient;
extern HWND			 g_hwnd;
extern float		 g_fSecPerFrame;
extern float		 g_fGameTimer;
extern POINT		 g_ptMouse;
extern BoxObj*		 g_pBoxDebug;
extern ID3D11Device* g_pd3dDevice;

typedef basic_string<TCHAR>				T_STR;
typedef basic_string<wchar_t>			W_STG;
typedef basic_string<char>				C_STR;
typedef vector<T_STR>					T_STR_VECTOR;
typedef basic_string<TCHAR>::iterator	T_ITOR;
typedef basic_string<wchar_t>::iterator	W_ITOR;
typedef basic_string<char>::iterator	C_ITOR;
typedef vector<T_STR>					T_ARRAY_ITOR;
typedef vector<DWORD>					DWORD_VECTOR;
typedef	vector< DWORD >::iterator		DWORD_VECTOR_ITOR;
typedef list<DWORD>						DWORD_LIST;
typedef list<DWORD>::iterator			DWORD_LIST_ITOR;
typedef list< HANDLE >					HANDLE_LIST;
typedef	list< HANDLE >::iterator		HANDLE_LIST_ITOR;

static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;//???????? <-> ??????????
	return std::wstring(A2W(_src.c_str()));//?????????? -> ????????
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));//???????? -> ??????????
};

template<class T>
class Singleton //?????? ?????????? ???? 1???? ???????? ????
{
public://T->????
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

static void DisplayText(const char* fmt, ...)
{
	//????????(?????????? ???? ??????????????)
	va_list arg;                    //?? ?????????? ???? ?????? ???????? ??????(????)
	va_start(arg, fmt);             //???????? ?? ?????? ?????? ?????? ??????(??????,????????)
	char buf[1024 + 256] = { 0, };  //
	vsprintf_s(buf, fmt, arg);      //?????? ???????? ?????? ???? ?????? ????????
	OutputDebugStringA((char*)buf); //?????? ?? ????

	va_end(arg);                    //?????? ???? ???? ????
}
static void MemoryReporting()
{
#if defined(DEBUG) | defined(_DEBUG)
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));
	IDXGIDebug* debug;
	GetDebugInterface(IID_PPV_ARGS(&debug));
	OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);//???? ???? ????
	OutputDebugStringW(L"Completed Live Direct3D Object Dump.\r\n");
	debug->Release();
#endif
}

#define GAME_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){   Sample core;   
#define GAME_WIN(s,x,y) if (core.SetWinClass(hInstance) == FALSE) return 1;   if (core.SetWindow(L#s, x, y) == FALSE) return 1;   core.GameRun();    return 1;}
#define SIMPLE_WIN() if (core.SetWinClass(hInstance) == FALSE) return 1;   if (core.SetWindow() == FALSE) return 1;   core.GameRun();    return 1;}

//GAME_START, GAME_WIN ?????????? ??????
#define GAME_RUN(s,x,y) GAME_START; GAME_WIN(s,x,y);

//GAME_START, SIMPLE_WIN ?????????? ??????
#define RUN() GAME_START; SIMPLE_WIN();

#define BEGIN_START(S) friend class Singleton<S>

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


/////////////////////////////////////////  ???????? ??????. ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H
//????????:?????? ?????????? ?????? ????(x?? ?????? ????)?? ???? ???? ???? ???? x=DX???????? ???????? ????.
#define H(x){ hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__), __LINE__);\
							MessageBox(NULL, buffer,output,MB_OK);}\
						}
#endif
#else
#ifndef H
#define H(x) (x)
#endif
#endif

/////////////////////////////////////////  ?????? ?????? ???????? ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H_RETURN
//????????:hr = (x); ?? ?????? ?????? ?????????? ?????? ????(x?? ?????? ????)?? ???? ???????? ??????.
#define H_RETURN(x){  hr = (x); if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
						}
#endif
#else
#define H_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; }}
#endif

/////////////////////////////////////////  ?????? ???????? ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef U_RETURN
//????????:hr = (x); ?? ?????? ?????? ?????????? ?????? ????(x?? ?????? ????)?? ???? ???????? ??????.
#define U_RETURN(x){  hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
							else { return hr;}\
						}
#endif
#else
#define U_RETURN(x){ hr = (x); return hr; }
#endif


#ifndef V
#define V(x)       { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef T_RETURN
#define T_RETURN(x,ret){ hr = (x); if( FAILED(hr) ) { return ret; } }
#endif


//////////////////////////////////////////////
// ???? ?? ???? ?????? ???? ?? ???? ??????
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif
