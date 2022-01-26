#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ChattingServer.h"
#include <ws2ipdef.h>
DWORD WINAPI WorkThread(LPVOID param)
{
    DWORD trans;
    ULONG_PTR key;
    OVERLAPPED* overlapped;
    ChattingServer* server = (ChattingServer*)param;
    while (1)
    {
        if (WaitForSingleObject(server->m_hKillEvent, 1) == WAIT_OBJECT_0)
        {
            break;
        }
        bool ret = GetQueuedCompletionStatus(server->m_hServer,&trans,&key,&overlapped,1);
        ChattingUser* user = (ChattingUser*)key;
        NewOV* nov = (NewOV*)overlapped;
        if (ret == true && user && nov)
        {
            if (trans == 0 && nov->type == 1000)
            {
                user->m_connect = false;
            }
            else
            {
                user->Dispatch(trans, nov);
            }
        }
        else
        {
            if (GetLastError() != WAIT_TIMEOUT)
            {
                SetEvent(server->m_hKillEvent);
                break;
            }
        }
              
    }
    return 1;
}
bool ChattingServer::InitServer(int port)
{
    Server::InitServer(port);
    m_hServer = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    for (int i = 0; i < THREAD_MAX; i++)
    {
        DWORD id;
        m_hThread[i] = CreateThread(0,0,WorkThread,this,0,&id);
    }
    return true;
}
bool ChattingServer:: Run()
{
    while (1)
    {
        EnterCriticalSection(&m_cs);
        list<User*>::iterator iter;
        for (iter = m_userlist.begin(); iter != m_userlist.end();)
        {
            ChattingUser* chatuser = (ChattingUser*)*iter;
            if (chatuser->m_packetPool.size() > 0)
            {
                Broadcast(*iter);
            }
            iter++;
        }
        for (iter = m_userlist.begin(); iter != m_userlist.end();)
        {
            if ((*iter)->m_connect == false)
            {
                ChattingUser* chatuser = (ChattingUser*)*iter;
                //clientaddr.sin_addr
                cout << "ip = " << chatuser->m_ip
                     << "  접속 해제!" << endl;
                delete(*iter);
                iter = m_userlist.erase(iter);
                cout << m_userlist.size() << "명 접속중입니다." << endl;
            }
            else
            {
                iter++;
            }
        }

        LeaveCriticalSection(&m_cs);
        Sleep(1);
    }
    return true;
}
bool ChattingServer:: Release()
{
    Server::Release();
    CloseHandle(m_hServer);
    return true;
}
bool ChattingServer::AddUser(SOCKET sock, SOCKADDR_IN clientaddr)
{
    ChattingUser* user = new ChattingUser;
    user->UserSet(sock, clientaddr);

    //넌블로킹
    u_long on = 1;
    ioctlsocket(sock, FIONBIO, &on);

    EnterCriticalSection(&m_cs);
    m_userlist.push_back(user);
    LeaveCriticalSection(&m_cs);

    CreateIoCompletionPort((HANDLE)sock, m_hServer, (ULONG_PTR)user, 0);
    user->Recv();

    cout << "ip = " << inet_ntoa(clientaddr.sin_addr)
        << " port = "<< ntohs(clientaddr.sin_port)
        <<"  " << endl;
    cout << m_userlist.size() << "명 접속중입니다." << endl;
    return true;
}
