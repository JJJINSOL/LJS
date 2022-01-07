#include "Window.h"
RECT g_rtClient;

//������ ���ν���, �������� ���� �޼��� ó�� �Լ�
//(1���� - �ڵ� / 2���� - �޼��� / 3���� - �ڵ�,���� �޾Ƶ��϶� / 4���� - �����Ͱ� ����) 
//WM = ������ �޼���
LRESULT  CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY: //WM_DESTORY = â�� ���� �� ���� ���α׷����� â�� �Ҵ��� �ڿ��� �����ϰų� ��ó�� �۾��� �� �� �ְ� �����ϴ� ������ �޽���
    {
        //�޽��� ť�� WM_QUIT �޽����� ����
        PostQuitMessage(0);
    }break;
    default:
        //DefWindowProc = �������� �������� ������ ó��(ex. â�� ũ�� ���, Ȯ�� ��)
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
//1. ������ Ŭ���� ��� / �㰡 (�ü��)
BOOL Window::SetWinClass(HINSTANCE hInstance)
{
    m_hInsatance = hInstance;
    WNDCLASS wc;

    //memset�� ���� ������Ŭ���� �޸� ����
    ZeroMemory(&wc, sizeof(WNDCLASS));

    //CS = Ŭ���� / CS_HREDRAW = �������� �̵��̳� ũ�� ������ Ŭ���̾�Ʈ�� ������ ũ�Ⱑ ����Ǹ� �ٽ� �׸�
    //              CS_VREDRAW = �������� �̵��̳� ũ�� ������ Ŭ���̾�Ʈ�� ������ ũ�Ⱑ ����Ǹ� �ٽ� �׸�
    //��, ���� ���� ũ�� ���� ����
    wc.style = CS_HREDRAW | CS_VREDRAW;

    //lp = ������ / �Լ� �ּ� / ������ ���ν���
    wc.lpfnWndProc = WndProc;

    //�ü���� �����ϴ� ��ȣ = �ֹε�Ϲ�ȣ
    wc.hInstance = hInstance;

    //sz = ��Ʈ�� / L = LONG / �ü�� ��� �̸� / ������ �̸� ��� �������� ���ƾ���
    wc.lpszClassName = L"KGCA_Window";
    if (RegisterClass(&wc) == false)
    {
        return FALSE;
    }
    return TRUE;
}
//2. ������ ����
BOOL Window::SetWindow(const WCHAR* szTitle, int iWidth, int iHeight)
{
    //������,����,����
    RECT rt = { 0,0, iWidth , iHeight };

    //������ âũ�� ���� ����
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

    //������ Ŭ���̾�Ʈ ���������� ��ǥ�� ��ȯ
    GetClientRect(m_hWnd, &m_rtClient);

    //������ ȭ�� ��ǥ ������ ��ġ ������ ��ȯ
    GetWindowRect(m_hWnd, &m_rtWindow);

    g_rtClient = m_rtClient;

    ShowWindow(m_hWnd, SW_SHOW);
    return TRUE;
}
bool Window::WinRun()
{
    MSG msg;

    //PeekMessage = �޼��� ť�� �޼����� �ִ��� ������ �˻��ϰ�, msg ����ü�� �޼����� �������ִ� �Լ�
    //�޼���ť�� �޼����� ������ ���
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);// �޼��� ����
        DispatchMessage(&msg);// ���������ν��� ȣ��
    }

    return true;
}