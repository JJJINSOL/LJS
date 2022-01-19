#include "ChattingServer.h"
#include <ws2ipdef.h>
#include <WS2tcpip.h>
bool ChattingServer::InitServer(int port)
{
    Server::InitServer(port);
    return true;
}

bool ChattingServer::AddUser(SOCKET sock, SOCKADDR_IN clientaddr)
{
    ChattingUser* user = new ChattingUser;
    user->UserSet(sock, clientaddr);

    //≥Õ∫Ì∑Œ≈∑
    u_long on = 1;
    ioctlsocket(sock, FIONBIO, &on);

    EnterCriticalSection(&m_cs);
    m_userlist.push_back(user);
    LeaveCriticalSection(&m_cs);

    CreateIoCompletionPort((HANDLE)sock, m_hServer, (ULONG_PTR)user, 0);

    char ip[INET_ADDRSTRLEN];
    cout << "ip = " << inet_ntop(AF_INET, &(clientaddr.sin_addr) ,ip, INET_ADDRSTRLEN)
        << " port = "<< ntohs(clientaddr.sin_port)
        <<"  " << endl;

    return true;
}
