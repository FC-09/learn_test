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

//�����׽��֣����ӷ�����
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
    std::cout << "������������еĵ�ǰĿ¼��" << std::endl;
    std::cin >> path;
    for (char i = 0; path[i] != 0; i++)
    {
        send_text_[i + 1] = path[i];
    }
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'f')
        std::cout << "Ŀ¼����ʧ�ܣ�" << std::endl;
    else if (recv_text_[0] == 'o')
        std::cout << "���л�����Ŀ¼-> " << path << std::endl;
    return;
}

void FileClient::GetDirectory()
{
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "δ���õ�ǰĿ¼! " << std::endl;
    else if (recv_text_[0] == 'f')
        std::cout << "δ�ҵ���Ŀ¼������������!" << std::endl;
    else if (recv_text_[0] == 'o')
    {
        char files_name[100];
        memset(files_name, 0, sizeof(char) * 100);
        for (char i = 1; recv_text_[i] != 0; i++)
        {
            files_name[i - 1] = recv_text_[i];
        }
        std::cout << "��ǰĿ¼���������ļ����ļ��У�" << files_name << std::endl;
    }
    return;
}

void FileClient::DisplayPath()
{
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "δ���õ�ǰĿ¼! " << std::endl;
    else if (recv_text_[0] == 'o')
    {
        char path[100];
        memset(path, 0, sizeof(char) * 100);
        for (char i = 1; recv_text_[i] != 0; i++)
        {
            path[i - 1] = recv_text_[i];
        }
        std::cout << "��ǰĿ¼Ϊ-> " << path << std::endl;
    }
    return;
}

//�����ļ�
void FileClient::DownloadFile()
{
    char file_name[100], file_path[100];
    memset(file_name, 0, sizeof(char) * 100);
    memset(file_path, 0, sizeof(char) * 100);
    std::cout << "������Ҫ���ص�Զ���ļ�" << std::endl;
    std::cin >> file_name;
    std::cout << "������Ҫ���صı���·��" << std::endl;
    std::cin >> file_path;
    for (char i = 0; file_name[i] != 0; i++)
    {
        send_text_[i + 1] = file_name[i];
    }
    send(sock_, send_text_, BUF_SIZE, 0);
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "δ���õ�ǰĿ¼! " << std::endl;
    else if (recv_text_[0] == 'f')
    {
        std::cout << "���ļ�ʧ��" << std::endl;
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
            std::cout << "�ļ����سɹ�" << std::endl;
        }
        else
        {
            std::cout << "�½��ļ�ʧ��" << std::endl;
        } 
    }
    return;
}

//�ϴ��ļ�
void FileClient::UploadFile()
{
    char file_name[100], file_path[100];
    memset(file_name, 0, sizeof(char) * 100);
    memset(file_path, 0, sizeof(char) * 100);
    std::cout << "������Ҫ�ϴ��ı����ļ�" << std::endl;
    std::cin >> file_name;
    std::cout << "������Ҫ�ϴ���Զ��·��" << std::endl;
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
        std::cout << "�ļ��򿪳ɹ�" << std::endl;
    }
    else
    {
        std::cout << "�ļ���ʧ��" << std::endl;
        return;
    }
    recv(sock_, recv_text_, BUF_SIZE, 0);
    if (recv_text_[0] == 'n')
        std::cout << "δ���õ�ǰĿ¼! " << std::endl;
    else if (recv_text_[0] == 'f')
    {
        std::cout << "�ļ��ϴ�ʧ��" << std::endl;
    }
    else if (recv_text_[0] == 'o')
    {
        std::cout << "�ļ��ϴ��ɹ�" << std::endl;
    }
    return;
}

void FileClient::Run()
{
    if (Init(sock_) == 0)
    {
        while (1)
        {
            std::cout << "���������" << std::endl;
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
                std::cout << "������������ѶϿ���" << std::endl;
                break;
            }
        }
    }
    return;
}