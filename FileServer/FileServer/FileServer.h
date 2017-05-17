#pragma once
#include <iostream>
#include <list>
#include <windows.h>
#include "FileClient.h"
#pragma comment(lib, "WS2_32.lib")

class FileServer
{
public:
    FileServer();
    ~FileServer();
    int Run();
    void RemoveClient(FileClient* client);
private:
    WSADATA wsaData;
    WORD sockVersion;
    SOCKET sListen;
    SOCKET sClient;
    sockaddr_in sin;
    sockaddr_in remoteAddr;
    int nAddrLen;
    std::list<FileClient*> client_mgr_;
};