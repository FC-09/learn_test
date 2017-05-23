#include "FileClient.h"
#include "FileServer.h"

FileClient::FileClient(FileServer* server, SOCKET sClient)
{
	printf("[0x%X] FileClient()\n", this);

    memset(path_, 0, sizeof(char) * 100);
    memset(recv_text_, 0, sizeof(char) * BUF_SIZE);
    memset(send_text_, 0, sizeof(char) * BUF_SIZE);
    client_ = sClient;
    server_ = server;
    std::thread recv(&FileClient::Run, this);
    recv.detach();
}

FileClient::~FileClient()
{
	printf("[0x%X] ~FileClient()\n", this);
}

int FileClient::SetPath()
{
    recv(client_, recv_text_, BUF_SIZE, 0);
    strcpy(path_, recv_text_);
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
    send(client_, send_text_, BUF_SIZE, 0);
    memset(recv_text_, 0, sizeof(char) * BUF_SIZE);
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
    send(client_, send_text_, BUF_SIZE, 0);
    memset(send_text_, 0, sizeof(char) * BUF_SIZE);
    return 0;
}

int FileClient::DisplayPath()
{
    if (path_ == 0)
    {
        send_text_[0] = 'n';
        return -1;
    }
    else
    {
        send_text_[0] = 'o';
        strcat(send_text_, path_);
        send(client_, send_text_, BUF_SIZE, 0);
        memset(send_text_, 0, sizeof(char) * BUF_SIZE);
        return 0;
    }
}

int FileClient::DownloadFile()
{
    if (path_ == 0)
    {
        send_text_[0] = 'n';
        return -1;
    }
	char recvBuf[128 * 1024] = {0};
    //recv(client_, recv_text_, BUF_SIZE, 0);
	recv(client_, recvBuf, sizeof(recvBuf), 0);
    //---------打开文件，保存并发送给客户端----------
    FILE *fr;
    if (fr = fopen(recvBuf/*recv_text_*/, "rb"))
    {
        send_text_[0] = 'o';
    }
    else
    {
        send_text_[0] = 'f';
        return -3;
    }
    send(client_, send_text_, BUF_SIZE, 0);
    memset(send_text_, 0, sizeof(char) * BUF_SIZE);
    while (1)
    {
		/* if (fread(send_text_, sizeof(char), BUF_SIZE, fr) == 0)
			 break;
			 send(client_, send_text_, BUF_SIZE, 0);*/
		int readLength = fread(send_text_, sizeof(char), BUF_SIZE, fr);
		if (readLength <= 0)
		{
			send(client_, "download ok", strlen("download ok"), 0);
			break;
		}

		send(client_, send_text_, readLength, 0);
    }
    fclose(fr);
    //memset(recv_text_, 0, sizeof(char) * BUF_SIZE);
    return 0;
}

int FileClient::UploadFile()
{
    if (path_ == 0)
    {
        send_text_[0] = 'n';
        return -1;
    }
    recv(client_, recv_text_, BUF_SIZE, 0);
    if (FILE *fw = fopen(recv_text_, "wb"))
    {
        while (1)
        {
            if (recv(client_, recv_text_, BUF_SIZE, 0) == 0)
                break;
            fwrite(recv_text_, sizeof(char), BUF_SIZE, fw);
        }
        fclose(fw);
        send_text_[0] = 'o';
    }
    else
    {
        send_text_[0] = 'f';
    }
    send(client_, send_text_, BUF_SIZE, 0);
    memset(recv_text_, 0, sizeof(char) * BUF_SIZE);
    memset(send_text_, 0, sizeof(char) * BUF_SIZE);
    return 0;
}

int FileClient::Run()
{
    while (1)
    {
		if (recv(client_, recv_text_, BUF_SIZE, 0) > 0)
		{
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
		else
			break;
    }
    server_->RemoveClient(this);
	printf("[0x%X] thread exit\n", this);
    return 0;
}