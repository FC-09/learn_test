#include "FileClient.h"
#include "FileServer.h"

FileClient::FileClient(FileServer* server, SOCKET sClient)
{
    memset(path_, 0, sizeof(char) * 100);
    memset(recv_text_, 0, sizeof(char) * RECV_BUF_SIZE);
    memset(send_text_, 0, sizeof(char) * SEND_BUF_SIZE);
    client_ = sClient;
    server_ = server;
    std::thread recv(&FileClient::Run, this);
    recv.detach();
}

FileClient::~FileClient()
{
    
}

int FileClient::SetPath()
{
    if (recv_text_[1] != 0)
    {
        for (char i = 1; recv_text_[i] != 0; i++)
        {
            path_[i - 1] = recv_text_[i];
        }
        if (!boost::filesystem::exists(path_))
        {
            memset(path_, 0, sizeof(char) * 100);
            send_text_[0] = 'f';
            return -2;
        }
        else
        {
            send_text_[0] = 'o';
            char* s_text = "当前目录已设置为-> ";
            std::cout << s_text << path_ << std::endl;
        }
    }
    send(client_, send_text_, SEND_BUF_SIZE, 0);
    memset(recv_text_, 0, sizeof(char) * RECV_BUF_SIZE);
    return 0;
}

int FileClient::GetDirectory()
{
    if (path_ == 0)
    {
        send_text_[0] = 'n';
        return -1;
    }
    else
    {
        char files_name[100];
        char m = 1;
        memset(files_name, 0, sizeof(char) * 100);
        boost::filesystem::directory_iterator item_begin(path_);
       /* if (item_begin == NULL)
        {

        }*/
        boost::filesystem::directory_iterator item_end;
        for (; item_begin != item_end; item_begin++)
        {
            //item_begin->path().c_str;
            sprintf(files_name, "%s", item_begin->path().filename().c_str());
            for (char i = 0; files_name[i] != 0; i++)
            {
                send_text_[m] = files_name[i];
                m++;
            }  
        }
        send_text_[0] = 'o';
    }
    send(client_, send_text_, SEND_BUF_SIZE, 0);
    memset(send_text_, 0, sizeof(char) * SEND_BUF_SIZE);
    return 0;
}

int FileClient::DownloadFile()
{
    if (path_ == 0)
    {
        send_text_[0] = 'n';
        return -1;
    }
    char file_name[100];
    memset(file_name, 0, sizeof(char) * 100);
    if (recv_text_[101] != 0)
    {
        for (char i = 1; recv_text_[i] != 0; i++)
        {
            file_name[i - 1] = recv_text_[i];
        }
    }
    //---------打开文件，保存并发送给客户端----------
    FILE *fr;
    if (fr = fopen(file_name, "rb"))
    {
        char file[100000];
        memset(file, 0, sizeof(char) * 100000);
        fgets(file, ftell(fr), fr);
        fclose(fr);
        for (char i = 203; send_text_[i] != 0; i++)
        {
            send_text_[i] = file[i - 203];
        }
        send_text_[0] = 'o';
    }
    else
    {
        send_text_[0] = 'f';
        return -3;
    }
    send(client_, send_text_, SEND_BUF_SIZE, 0);
    memset(recv_text_, 0, sizeof(char) * RECV_BUF_SIZE);
    return 0;
}

int FileClient::UploadFile()
{
    if (path_ == 0)
    {
        send_text_[0] = 'n';
        return -1;
    }
    char* file_path = 0; char* file = 0;
    if (recv_text_[1] != 0)
    {
        for (char i = 1; recv_text_[i] != 0; i++)
        {
            *(file_path + i - 1) = recv_text_[i];
        }
    }
    if (recv_text_[101] != 0)
    {
        for (char i = 101; recv_text_[i] != 0; i++)
        {
            *(file + i - 101) = recv_text_[i];
        }
    }
    //----------保存文件，上传到指定路径------------
    FILE *fw;
    if (fw = fopen(file_path, "wb"))
    {
        char file[100000];
        memset(file, 0, sizeof(char) * 100000);
        for (char i = 101; send_text_[i] != 0; i++)
        {
            send_text_[i] = file[i - 101];
        }
        fputs(file, fw);
        fclose(fw);
        send_text_[0] = 'o';
    }
    else
    {
        send_text_[0] = 'f';
    }
    send(client_, send_text_, SEND_BUF_SIZE, 0);
    return 0;
}

int FileClient::DisplayPath()
{
    if (path_ == 0)
    {
        send_text_[0] = 'n';
        return -1;
    }
    send(client_, path_, strlen(path_), 0);
    return 0;
}

int FileClient::Run()
{
    while (1)
    {
        recv(client_, recv_text_, RECV_BUF_SIZE, 0);
        if (recv_text_[0] == 'S')
        {
            SetPath();
        }
        else if (recv_text_[0] == 'L')
        {
            GetDirectory();
        }
        else if (recv_text_[0] == 'D')
        {
            DownloadFile();
        }
        else if (recv_text_[0] == 'P')
        {
            UploadFile();
        }
        else if (recv_text_[0] == 'C')
        {
            DisplayPath();
        }
        else if (recv_text_[0] == 'Q')      //若客户端发送“Q”则表示断开连接
            break;
    }
    server_->RemoveClient(this);
    return 0;
}