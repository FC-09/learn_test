#include "Boost/asio.hpp"
#include <iostream>

class BoostAsyncClient
{
public:
    BoostAsyncClient()
        :ep_(boost::asio::ip::address::from_string("127.0.0.1"), 1234)
    {
        memset(recv_text_, 0, 20);
    }
    ~BoostAsyncClient(){}

    int Run()
    {
        StartConnect();
        return ios_.run();
    }

    void StartConnect()
    {
        boost::shared_ptr<boost::asio::ip::tcp::socket> client(new boost::asio::ip::tcp::socket(ios_));
        client->async_connect(ep_, std::bind(&BoostAsyncClient::ConnectHandler, this, client, std::placeholders::_1));
    }

    void ConnectHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client, boost::system::error_code ec)
    {
        if (ec)
            return;
        std::cout << "recive from:" << client->remote_endpoint().address() << std::endl;;
        std::cout << " port:" << client->remote_endpoint().port() << std::endl;
        char text[20] = { 0 };
        client->async_receive(boost::asio::buffer(text, sizeof(text)), 
            bind(&BoostAsyncClient::ReadHandler, this, std::placeholders::_1, std::placeholders::_2));
        memcpy(recv_text_, text, strlen(text));
        StartConnect();
    }

    void ReadHandler(boost::system::error_code ec, size_t bytes_transferred)
    {
        if (ec)
        {
            return;
        }
        int len = strlen(recv_text_);
        for (int i = 0; i < len; ++i)
            std::cout << recv_text_[i];
        std::cout << std::endl;
    }
private:
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::endpoint ep_;
    char recv_text_[20];
};

int main()
{
    BoostAsyncClient client;
    client.Run();
    return 0;
}