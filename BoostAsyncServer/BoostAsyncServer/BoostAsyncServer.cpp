#include <iostream>
#include "boost/asio.hpp" 
#include "boost/shared_ptr.hpp"

class BoostAsyncServer
{
public:
    BoostAsyncServer()
        : acc_(ios_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234)) {}
    ~BoostAsyncServer(){}

    int Run()
    {
        StartAccept();
        return ios_.run();
    }

    void StartAccept()
    {
        boost::shared_ptr<boost::asio::ip::tcp::socket> client(new boost::asio::ip::tcp::socket(ios_));
        acc_.async_accept(*client, std::bind(&BoostAsyncServer::AcceptHandler, this, client, std::placeholders::_1));
    }

    void AcceptHandler(boost::shared_ptr<boost::asio::ip::tcp::socket> client, boost::system::error_code ec)
    {
        if (ec)
            return;
        client->async_send(boost::asio::buffer("hello world", sizeof("hello world")),
            std::bind(&BoostAsyncServer::WriteHandler, this, std::placeholders::_1, std::placeholders::_2));
        StartAccept();
    }

    void WriteHandler(boost::system::error_code ec, size_t bytes_transferred)
    {
        if (ec)
            std::cout << "发送失败" << std::endl;
        else
            std::cout << "发送成功" << std::endl;
    }

private:
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::acceptor acc_;
};

int main()
{
    BoostAsyncServer server;
    server.Run();
    return 0;
}