#include "BoostAsyncServer.h"

BoostAsyncServer::BoostAsyncServer()
    : acc_(ios_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234)){}

BoostAsyncServer::~BoostAsyncServer()
{
    clients_.clear();
}

int BoostAsyncServer::Run()
{
    StartAccept();
    return ios_.run();
}

void BoostAsyncServer::StartAccept()
{
    boost::shared_ptr<boost::asio::ip::tcp::socket> p_sock(new boost::asio::ip::tcp::socket(ios_));    
    acc_.async_accept(*p_sock, std::bind(&BoostAsyncServer::AcceptHandler, this, p_sock, std::placeholders::_1));
}

void BoostAsyncServer::AcceptHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& p_sock, boost::system::error_code ec)
{
    if (ec)
        return;
    std::cout << "recive from:" << p_sock->remote_endpoint().address() << std::endl;
    std::cout << " port:" << p_sock->remote_endpoint().port() << std::endl;    
    clients_.push_back(p_sock);
    std::shared_ptr<Client> client(new Client(this, p_sock));
    client->Run();
    StartAccept();
}

void BoostAsyncServer::RemoveClient(boost::shared_ptr<boost::asio::ip::tcp::socket>& client)
{
    clients_.remove(client);
}

//void BoostAsyncServer::SendHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
//    boost::system::error_code ec, size_t bytes_transferred)
//{
//    if (ec)
//        std::cout << "发送失败" << std::endl;
//    else
//    {
//        std::cout << "发送成功" << std::endl;
//        client->async_send(boost::asio::buffer("hello world", sizeof("hello world")),
//            std::bind(&BoostAsyncServer::SendHandler, this, client, std::placeholders::_1, std::placeholders::_2));
//    }
//}
//
//void BoostAsyncServer::RecvHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
//    boost::system::error_code ec, size_t bytes_transferred)
//{
//    if (ec)
//    {
//        clients_.remove(client);
//        std::cout << "接收失败" << std::endl;
//        return;
//    }
//    else
//    {
//        std::cout << "接收成功" << std::endl;
//        memcpy(file_path_, recv_text_ + 1, recv_text_[0]);
//        File_Download();
//        client->async_send(boost::asio::buffer(send_text_, sizeof(send_text_)),
//            std::bind(&BoostAsyncServer::SendHandler, this, client, std::placeholders::_1, std::placeholders::_2));
//    }
//}