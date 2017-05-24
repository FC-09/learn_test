#include "Client.h"
#include "BoostServer.h"

Client::Client(BoostServer* server, boost::asio::ip::tcp::socket* client)
    :client_(client)
{
    printf("[0x%X] FileClient()\n", this);
    server_ = server;
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
        char buf = { 0 };
        boost::system::error_code ec;
        client_->receive(boost::asio::buffer(&buf, 1), 0, ec);
        if (ec.value() != 0)
            break;
        //std::cout << buf_ << std::endl;
        else if (buf == 'D')
            {
                DownloadFile();
            }
        //else if (buf_[0] == 'P')
        //    {
        //        UploadFile();
        //    }
        //else if (buf_[0] == 'Q')      //若客户端发送“Q”则表示断开连接
        //        break;
    }
    server_->RemoveClient(this);
    printf("[0x%X] thread exit\n", this);
    return 0;
}


int Client::DownloadFile()
{
    char recvBuf[100] = { 0 };
    client_->receive(boost::asio::buffer(recvBuf, sizeof(recvBuf)), 0);
    FILE *fr;
    if (fr = fopen(recvBuf, "rb"))
    {
        char text_buf[100] = { 0 };
        while (1)
        {
            int readLength = fread(text_buf, sizeof(char), 100, fr);
            if (readLength <= 0)
            {
                char* ok = "download ok";
                client_->send(boost::asio::buffer(ok, sizeof("download ok")), 0);
                break;
            }   
            client_->send(boost::asio::buffer(text_buf, readLength), 0);
        }
    }
    fclose(fr);
    return 0;
}


int Client::UploadFile()
{

    return 0;
}