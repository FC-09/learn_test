#include <iostream>
#include "boost/asio.hpp" 
#include "boost/shared_ptr.hpp"
#include <list>

class BoostAsyncServer
{
public:
    BoostAsyncServer()
        : acc_(ios_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234)) {}

    ~BoostAsyncServer()
    {
        clients_.clear();
    }

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

    void AcceptHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client, boost::system::error_code ec)
    {
        if (ec)       
            return;  
        std::cout << "recive from:" << client->remote_endpoint().address() << std::endl;;
        std::cout << " port:" << client->remote_endpoint().port() << std::endl;
        clients_.push_back(client);
        client->async_send(boost::asio::buffer("hello world", sizeof("hello world")),
            std::bind(&BoostAsyncServer::SendHandler, this, client, std::placeholders::_1, std::placeholders::_2));
        client->async_receive(boost::asio::buffer(recv_text_, sizeof(recv_text_)),
            std::bind(&BoostAsyncServer::RecvHandler, this, client, std::placeholders::_1, std::placeholders::_2));
        StartAccept();
    }

    void SendHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client, 
        boost::system::error_code ec, size_t bytes_transferred)
    {
        if (ec)      
            std::cout << "发送失败" << std::endl;       
        else
        {
            std::cout << "发送成功" << std::endl;
            client->async_send(boost::asio::buffer("hello world", sizeof("hello world")),
                std::bind(&BoostAsyncServer::SendHandler, this, client, std::placeholders::_1, std::placeholders::_2));
        }            
    }

    void RecvHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client, 
        boost::system::error_code ec, size_t bytes_transferred)
    {
        if (ec)            
        {
            clients_.remove(client);
            std::cout << "接收失败" << std::endl;
            return;
        }
        else
        {
            std::cout << "接收成功" << std::endl;
            client->async_receive(boost::asio::buffer(recv_text_, sizeof(recv_text_)),
                std::bind(&BoostAsyncServer::RecvHandler, this, client, std::placeholders::_1, std::placeholders::_2));
        }
    }

private:
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::acceptor acc_;
    char recv_text_[30 * 1024];
    std::list<boost::shared_ptr<boost::asio::ip::tcp::socket>> clients_;
};

int main()
{
    BoostAsyncServer server;
    server.Run();
    return 0;
}