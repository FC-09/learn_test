#include "FileServer.h"

FileServer::FileServer()
{
    sListen = 0;
    sClient = INADDR_ANY;
    sin = { 0 };
    remoteAddr = { 0 };
    sockVersion = MAKEWORD(2, 2);
    nAddrLen = sizeof(sockaddr_in);
    sin.sin_port = htons(1234);
    sin.sin_family = AF_INET;
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        std::cout << "initlization failed !" << std::endl;
        exit(0);
    }
}

FileServer::~FileServer()
{
    closesocket(sClient);
    closesocket(sListen);
    WSACleanup();
    for (auto it = client_mgr_.begin(); it != client_mgr_.end(); ++it)
        delete *it;
    client_mgr_.clear();  //删除所有客户端
}

int FileServer::Run()
{
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        std::cout << "bind failed !" << std::endl;
        return 0;
    }
    if (listen(sListen, 2) == SOCKET_ERROR)
    {
        std::cout << "listen failed!" << std::endl;
        return 0;
    }
    while (1)
    {
        sClient = accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
        if (sClient == INVALID_SOCKET)
        {
            std::cout << "accept failed !" << std::endl;
            continue;
        }
        FileClient *client = new FileClient(this, sClient);
        client_mgr_.push_front(client);
    }
    return 0;
}

void FileServer::RemoveClient(FileClient* client)
{
    client_mgr_.remove(client);   //删除指定客户端
    delete client;    //delete
}