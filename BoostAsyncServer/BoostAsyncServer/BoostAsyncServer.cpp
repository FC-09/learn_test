#include "BoostAsyncServer.h"

BoostAsyncServer::BoostAsyncServer()
    : acc_(ios_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234)){}

BoostAsyncServer::~BoostAsyncServer()
{
    //clients_.clear();
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
    std::cout << "port:" << p_sock->remote_endpoint().port() << std::endl;
    std::make_shared<Client>(this, p_sock)->Run();
    //std::shared_ptr<Client>((new Client(this, p_sock)))->Run();
    StartAccept();
}

void BoostAsyncServer::RemoveClient(std::shared_ptr<Client> client)
{
    //clients_.remove(client);
}