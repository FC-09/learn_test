#include "FileClient.h"

FileClient::FileClient()
    :sock_(0)
{
    sock_version_ = MAKEWORD(2, 2);
    memset(recv_text_, 0, sizeof(char) * BUF_SIZE);
    memset(send_text_, 0, sizeof(char) * BUF_SIZE);
}

FileClient::~FileClient()
{
    closesocket(sock_);
    WSACleanup();
}

//创建套接字，连接服务器
int FileClient::Init(SOCKET &sock)
{
    if (WSAStartup(sock_version_, &wsa_data_) != 0)
    {
        std::cout << "initlization failed!" << std::endl;
        return -1;
    }
    sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        std::cout << "failed socket!" << std::endl;
        return -1;
    }
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(1234);
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (connect(sock, (sockaddr*)&sin, sizeof(sockaddr)) == -1)
    {
        std::cout << "connect failed!" << std::endl;
        return -1;
    }
    return 0;
}

void FileClient::SetPath()
{
    char path[100];
    memset(path, 0, sizeof(char) * 100);
    std::cout << "请输入服务器中的当前目录：" << std::endl;
    std::cin >> path;
    for (char i = 0; path[i] != 0; i++)
    {
        send_text_[i + 1] = path[i];
    }
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'f')
        std::cout << "目录设置失败！" << std::endl;
    else if (recv_text_[0] == 'o')
        std::cout << "已切换到到目录-> " << path << std::endl;
    return;
}

void FileClient::GetDirectory()
{
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "未设置当前目录! " << std::endl;
    else if (recv_text_[0] == 'f')
        std::cout << "未找到该目录，请重新设置!" << std::endl;
    else if (recv_text_[0] == 'o')
    {
        char files_name[100];
        memset(files_name, 0, sizeof(char) * 100);
        for (char i = 1; recv_text_[i] != 0; i++)
        {
            files_name[i - 1] = recv_text_[i];
        }
        std::cout << "当前目录下有以下文件和文件夹：" << files_name << std::endl;
    }
    return;
}

void FileClient::DisplayPath()
{
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "未设置当前目录! " << std::endl;
    else if (recv_text_[0] == 'o')
    {
        char path[100];
        memset(path, 0, sizeof(char) * 100);
        for (char i = 1; recv_text_[i] != 0; i++)
        {
            path[i - 1] = recv_text_[i];
        }
        std::cout << "当前目录为-> " << path << std::endl;
    }
    return;
}

//下载文件
void FileClient::DownloadFile()
{
    char file_name[100], file_path[100];
    memset(file_name, 0, sizeof(char) * 100);
    memset(file_path, 0, sizeof(char) * 100);
    std::cout << "请输入要下载的远程文件" << std::endl;
    std::cin >> file_name;
    std::cout << "请输入要下载的本地路径" << std::endl;
    std::cin >> file_path;
    for (char i = 0; file_name[i] != 0; i++)
    {
        send_text_[i + 1] = file_name[i];
    }
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "未设置当前目录! " << std::endl;
    else if (recv_text_[0] == 'f')
    {
        std::cout << "打开文件失败" << std::endl;
    }
    else if (recv_text_[0] == 'o')
    {
        if (FILE *fw = fopen(file_path, "wb"))
        {
            while (1)
            {
                if (recv(sock_, recv_text_, BUF_SIZE, 0) == 0)
                    break;
                fwrite(recv_text_, sizeof(char), 100, fw);
            }
            fclose(fw);
            std::cout << "文件下载成功" << std::endl;
        }
        else
        {
            std::cout << "新建文件失败" << std::endl;
        } 
    }
    return;
}

//上传文件
void FileClient::UploadFile()
{
    char file_name[100], file_path[100];
    memset(file_name, 0, sizeof(char) * 100);
    memset(file_path, 0, sizeof(char) * 100);
    std::cout << "请输入要上传的本地文件" << std::endl;
    std::cin >> file_name;
    std::cout << "请输入要上传的远程路径" << std::endl;
    std::cin >> file_path;
    send(sock_, file_path, 100, 0);
    if (FILE *fr = fopen(file_name, "rb"))
    {
        while (1)
        {
            if (fread(send_text_, sizeof(char), BUF_SIZE, fr) == 0)
                break;
            send(sock_, send_text_, BUF_SIZE, 0);
        }
        fclose(fr);
        std::cout << "文件打开成功" << std::endl;
    }
    else
    {
        std::cout << "文件打开失败" << std::endl;
        return;
    }
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "未设置当前目录! " << std::endl;
    else if (recv_text_[0] == 'f')
    {
        std::cout << "文件上传失败" << std::endl;
    }
    else if (recv_text_[0] == 'o')
    {
        std::cout << "文件上传成功" << std::endl;
    }
    return;
}

void FileClient::Run()
{
    if (Init(sock_) == 0)
    {
        while (1)
        {
            std::cout << "请输入命令：" << std::endl;
            std::cin >> send_text_;
            if (send_text_[0] == 'S')
            {
                SetPath();
            }
            else if (send_text_[0] == 'L')
            {
                GetDirectory();
            }
            else if (send_text_[0] == 'D')
            {
                DownloadFile();
            }
            else if (send_text_[0] == 'P')
            {
                UploadFile();
            }
            else if (send_text_[0] == 'C')
            {
                DisplayPath();
            }
            else if (send_text_[0] == 'Q')
            {
                std::cout << "与服务器连接已断开！" << std::endl;
                break;
            }
        }
    }
    return;
}