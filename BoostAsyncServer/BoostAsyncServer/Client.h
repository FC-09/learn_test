#pragma once
#include "boost/asio.hpp" 
#include "boost/shared_ptr.hpp"
#include <iostream>
#define  BUF_SIZE  30*1024
class BoostAsyncServer;
class Client
{
public:
    Client(BoostAsyncServer*, boost::shared_ptr<boost::asio::ip::tcp::socket>&);
    ~Client();
    void Run();
    void RecvHandler(boost::system::error_code ec, size_t bytes_transferred);
    void SendHandler(boost::system::error_code ec, size_t bytes_transferred);
    void FileDownload();

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> p_client_;
    BoostAsyncServer* p_server_;
    char recv_text_[BUF_SIZE];
    char send_text_[BUF_SIZE];
    char file_path_[256];
    int read_len_;
    FILE* f;
};