#include <iostream>
#include <stdio.h>
#include <thread>
#include <windows.h>
#include "../../public/common.h"
#pragma comment(lib, "WS2_32.lib")

class FileClient
{
public:
    FileClient();
    ~FileClient();
    void Run();
    int Init(SOCKET &sock);
    void SetPath();
    void GetDirectory();
    void DisplayPath();
    void DownloadFile();
    void UploadFile();

private:
    SOCKET sock_;
    WSADATA wsa_data_;
    WORD sock_version_;
    char recv_text_[RECV_BUF_SIZE];
    char send_text_[SEND_BUF_SIZE];
};

