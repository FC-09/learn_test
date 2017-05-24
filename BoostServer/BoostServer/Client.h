#pragma once
#include "boost/asio.hpp"
#include <iostream>

#define BUF_SIZE     100

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
    BoostServer* server_;
    //char buf_[BUF_SIZE];
};