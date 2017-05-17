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
    int SetPath();           //����Ŀ¼��S��  ���ش�����
    int GetDirectory();                //��ȡĿ¼���ļ���L��  ���ش�����
    int DownloadFile();           //�����ļ���D��  ���ش�����
    int UploadFile();             //�ϴ��ļ���P��  ���ش�����
    int DisplayPath();                 //��ʾ·����C��  ���ش�����
    int Run();                         //���к���       ���ش�����

private:
    FileServer* server_;
    SOCKET client_;                    //�ͻ���
    char recv_text_[RECV_BUF_SIZE];           //����ָ��  recv_text[0]���ָ��   recv_text_[1]---recv_text_[100]���·��   recv_text_[102]---recv_text_[201]����ļ���
    char send_text_[SEND_BUF_SIZE];           //���ʹ����뼰�ļ����ͻ���   
    //send_text_[0]��Ŵ�����    send_text_[1]---send_text_[100]����ļ���   send_text_[102]---send_text_[201]����ļ�    send_text_[203]---send_text_[100202]����ļ����ļ���С����100kb��
    char path_[100];                  //Ŀ¼��
};