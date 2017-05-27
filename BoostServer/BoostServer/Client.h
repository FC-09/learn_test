#pragma once
#include "boost/asio.hpp"
#include <iostream>

#define BUF_SIZE     101

class BoostServer;
class Client
{
public:
    Client(BoostServer*, boost::asio::ip::tcp::socket*);
    ~Client();
    int Run();
    int DownloadFile();
    int UploadFile();

private:
    boost::asio::ip::tcp::socket* client_;
    boost::system::error_code ec_;
    BoostServer* server_;
    //char recv_buf_[BUF_SIZE];
    //char send_buf_[BUF_SIZE];
};