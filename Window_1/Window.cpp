#include "Window.h"
//윈도우 프로시저, 윈도우의 각종 메세지 처리 함수
//(1인자 - 핸들 / 2인자 - 메세지 / 3인자 - 핸들,정수 받아들일때 / 4인자 - 포인터값 전달) 
//WM = 윈도우 메세지
LRESULT  CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY: //WM_DESTORY = 창을 닫을 때 응용 프로그램에서 창에 할당한 자원을 해제하거나 후처리 작업을 할 수 있게 전달하는 윈도우 메시지
    {
        //메시지 큐에 WM_QUIT 메시지를 붙임
        //큐에 WM_QUIT를 붙임으로써 시스템에게 이 스레드가 종료될것을 알려줌
        PostQuitMessage(0);
    }break;
    default:
        //DefWindowProc = 윈도우의 공통적인 동작을 처리(ex. 창의 크기 축소, 확대 등)
        //WndProc에서 처리하지 않은 나머지 메시지에 관한 처리
        //그래서 윈도우의 이동, 크기 변경 같은 것들은 처리를 직접 해줄 필요가 없거 얘한테 넘기기
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
//1. 윈도우 클래스 등록 / 허가 (운영체제)
BOOL Window::WinRegisterClass(HINSTANCE hInstance)
{
    m_hInsatance = hInstance;
    WNDCLASS wc;

    //memset과 같이 윈도우클래스 메모리 세팅
    ZeroMemory(&wc, sizeof(WNDCLASS));

    //CS = 클래스 / CS_HREDRAW = 윈도우의 이동이나 크기 조절시 클라이언트의 가로의 크기가 변경되면 다시 그림
    //              CS_VREDRAW = 윈도우의 이동이나 크기 조절시 클라이언트의 세로의 크기가 변경되면 다시 그림
    //즉, 가로 세로 크기 변경 가능
    wc.style = CS_HREDRAW | CS_VREDRAW;

    //lp = 포인터 / 함수 주소 / 윈도우 프로시저
    wc.lpfnWndProc = WndProc;

    //운영체제가 관리하는 번호 = 주민등록번호
    wc.hInstance = hInstance;

    //sz = 스트링 / L = LONG / 운영체제 등록 이름 / 설정된 이름 모두 곳곳마다 같아야함
    wc.lpszClassName = L"KGCA_Window";
    if (RegisterClass(&wc) == false)
    {
        return FALSE;
    }
    return TRUE;
}
//2. 윈도우 생성
BOOL Window::SetWindow(const WCHAR* szTitle, int iWidth, int iHeight )
{
    //left / top / right / bottom
    RECT rt = { 0,0, iWidth , iHeight };

    //윈도우 창크기 조정 세팅
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

    //(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
    m_hWnd = CreateWindow(
        L"KGCA_Window",
        szTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rt.right - rt.left,
        rt.bottom - rt.top,
        NULL,
        NULL,
        m_hInsatance,
        NULL);
    if (m_hWnd == NULL)
    {
        return FALSE;
    }
    
    //윈도우 클라이언트 영역에서의 좌표값 반환
    GetClientRect(m_hWnd, &m_rtClient);
    
    //윈도우 화면 좌표 값으로 위치 정보를 반환
    GetWindowRect(m_hWnd, &m_rtWindow);

    ShowWindow(m_hWnd, SW_SHOW);
    return TRUE;
}
bool Window:: WinRun()
{
    MSG msg;
    // 기본 메시지 루프
    while (1)
    {
        //PeekMessage = 메세지 큐에 메세지가 있는지 없는지 검사하고, msg 구조체에 메세지를 저장해주는 함수
        //메세지큐에 메세지가 없으면 블록
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);// 메세지 번역
            DispatchMessage(&msg);// 윈도우프로시져 호출
        }
        else
        {
            // 게임로직 호출
            static int count = 0;
            count++;
        }
    }
    //MessageBox(NULL, L"정말 종료 할 꺼야", L"kgca", MB_YESNOCANCEL);
    return false;
}