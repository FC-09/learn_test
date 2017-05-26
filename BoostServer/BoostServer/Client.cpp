#include "Client.h"
#include "BoostServer.h"

Client::Client(BoostServer* server, boost::asio::ip::tcp::socket* client)
    :client_(client)
{
    printf("[0x%X] FileClient()\n", this);
    memset(recv_buf_, 0, BUF_SIZE);
    memset(send_buf_, 0, BUF_SIZE);
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
        boost::system::error_code ec;
        client_->receive(boost::asio::buffer(recv_buf_, 102), 0, ec);
        if (ec.value() != 0)
            break;
        else if (recv_buf_[0] == 'D')
            {
                DownloadFile();
            }
        else if (recv_buf_[0] == 'U')
            {
                UploadFile();
            }
        else if (recv_buf_[0] == 'Q')      //若客户端发送“Q”则表示断开连接
                break;
    }
    server_->RemoveClient(this);
    printf("[0x%X] thread exit\n", this);
    return 0;
}


int Client::DownloadFile()
{
    char file_path[100] = { 0 };
    strcpy(file_path, recv_buf_ + 2);
    FILE *fr;
    if (fr = fopen(file_path, "rb"))
    {
        while (1)
        {
            char text_buf[100] = { 0 };
            char text[101] = { 0 };
            char readLength = fread(text_buf, sizeof(char), 100, fr);
            text[0] = readLength;
            //strcpy(text + 1, text_buf);
            memcpy(text + 1, text_buf, readLength);
            if ((int)readLength <= 0)
            {
                char* ok = "ok";
                client_->send(boost::asio::buffer(ok, sizeof("ok")), 0);
                break;
            }   
            client_->send(boost::asio::buffer(text, readLength + 1), 0);
            std::cout << (readLength + 1) << std::endl;
        }
    }
    fclose(fr);
    return 0;
}


int Client::UploadFile()
{
    
    return 0;
}


//void Client::Recv()
//{
//    while (1)
//    {
//        char recvBuf[100] = { 0 };
//        client_->receive(boost::asio::buffer(recvBuf, sizeof(recvBuf)), 0);
//           char recvBuf[100] = { 0 };
//    client_->recvive(boost::asio::buffer(recvBuf, sizeof(recvBuf)), );
//    }
//    
//
//}
//
//
//void Client::Send()
//{
//
//}