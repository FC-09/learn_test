#include "Client.h"
#include "BoostServer.h"
#include <stdio.h>


Client::Client(BoostServer* server, boost::asio::ip::tcp::socket* client)
    :client_(client)
{
    printf("[0x%X] FileClient()\n", this);
    /*memset(recv_buf_, 0, BUF_SIZE);
    memset(send_buf_, 0, BUF_SIZE);*/
    server_ = server;
	boost::asio::socket_base::send_buffer_size option(64*1024);
	client_->set_option(option);
    std::thread thread_run(&Client::Run, this);
    thread_run.detach();
}


Client::~Client()
{
    printf("[0x%X] ~FileClient()\n", this);
}


int Client::Run()
{
    while (1)
    {
        char order = { 0 };
        client_->receive(boost::asio::buffer(&order, 1), 0, ec_);
        if (ec_.value() != 0)
            break;
        else if (order == 'D')
            {
                DownloadFile();
            }
        else if (order == 'U')
            {
                UploadFile();
            }
        else if (order == 'Q')      //若客户端发送“Q”则表示断开连接
                break;
    }
    server_->RemoveClient(this);
    printf("[0x%X] thread exit\n", this);
    return 0;
}


int Client::DownloadFile()
{
    char file_path[256] = { 0 };
    client_->receive(boost::asio::buffer(file_path, 256), 0, ec_);
    if (FILE *fr = fopen(file_path, "rb"))
    {
        while (1)
        {
			char text_buf[SEND_BUF_SIZE + 4] = { 0 };
			unsigned int readLength = fread(text_buf + 4, sizeof(char), SEND_BUF_SIZE, fr);
            //std::cout << readLength << std::endl;
            memcpy(text_buf, &readLength, sizeof(int));
			//Sleep(100);
            int a = client_->send(boost::asio::buffer(text_buf, readLength + 4), 0, ec_);
            //std::cout << a << std::endl;
			static int sendCount = 0;
			sendCount++;
			static int totalSendLength = 0;
			totalSendLength += a;

			printf("send, readLength = %d, sendLength = %d, %d, total sendLength = %d, ec = %d\n", readLength, a, sendCount, totalSendLength, ec_.value());
            if (readLength <= 0)
                break;
        }
        fclose(fr);
    } 
    return 0;
}


int Client::UploadFile()
{
    
    return 0;
}