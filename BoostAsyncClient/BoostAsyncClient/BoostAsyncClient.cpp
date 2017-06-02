#include "BoostAsyncClient.h"


BoostAsyncClient::BoostAsyncClient()
    :ep_(boost::asio::ip::address::from_string("127.0.0.1"), 1234)
{
    memset(recv_text_, 0, sizeof(recv_text_));
}
BoostAsyncClient::~BoostAsyncClient(){}

int BoostAsyncClient::Run()
{
    StartConnect();
    return ios_.run();
}

void BoostAsyncClient::StartConnect()
{
    boost::shared_ptr<boost::asio::ip::tcp::socket> client(new boost::asio::ip::tcp::socket(ios_));
    client->async_connect(ep_, std::bind(&BoostAsyncClient::ConnectHandler, this, client, std::placeholders::_1));
}

void BoostAsyncClient::ConnectHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client, boost::system::error_code ec)
{
    if (ec)
        return;
    /*std::cout << "recive from:" << client->remote_endpoint().address() << std::endl;;
    std::cout << " port:" << client->remote_endpoint().port() << std::endl;*/
    client->async_receive(boost::asio::buffer(recv_text_, sizeof(recv_text_)),
        bind(&BoostAsyncClient::ReadHandler, this, client, std::placeholders::_1, std::placeholders::_2));
    std::cout << client.use_count() << std::endl;
    //StartConnect();
}

void BoostAsyncClient::ReadHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
    boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec)
        return;
    for (int i = 0; i < strlen(recv_text_); ++i)
        std::cout << recv_text_[i];
    std::cout << std::endl;
    memset(recv_text_, 0, sizeof(recv_text_));
    client->async_receive(boost::asio::buffer(recv_text_, sizeof(recv_text_)),
        bind(&BoostAsyncClient::ReadHandler, this, client, std::placeholders::_1, std::placeholders::_2));
}

int BoostAsyncClient::WriteFile()
{
    int len = 0;
    if (FILE* f = fopen("D:\\111.zip", "wb"))
    {
        fwrite(recv_text_, sizeof(char), sizeof(recv_text_), f);
        fclose(f);
    }
    return len;
}

int BoostAsyncClient::ReadFile()
{
    return 0;
}