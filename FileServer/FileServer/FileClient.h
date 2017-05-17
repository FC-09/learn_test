#pragma once
#include <iostream>
#include <windows.h>
#include <thread>
#include <string>
#include <mutex>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include "../public/common.h"
#pragma comment(lib, "WS2_32.lib")

class FileServer;
class FileClient
{
public:
    FileClient(FileServer*, SOCKET);
    ~FileClient();
    int SetPath();           //设置目录（S）  返回错误码
    int GetDirectory();                //获取目录下文件（L）  返回错误码
    int DownloadFile();           //下载文件（D）  返回错误码
    int UploadFile();             //上传文件（P）  返回错误码
    int DisplayPath();                 //显示路径（C）  返回错误码
    int Run();                         //运行函数       返回错误码

private:
    FileServer* server_;
    SOCKET client_;                    //客户端
    char recv_text_[RECV_BUF_SIZE];           //接收指令  recv_text[0]存放指令   recv_text_[1]---recv_text_[100]存放路径   recv_text_[102]---recv_text_[201]存放文件名
    char send_text_[SEND_BUF_SIZE];           //发送错误码及文件给客户端   
    //send_text_[0]存放错误码    send_text_[1]---send_text_[100]存放文件名   send_text_[102]---send_text_[201]存放文件    send_text_[203]---send_text_[100202]存放文件（文件大小限制100kb）
    char path_[100];                  //目录名
};