#include "BoostServer.h"

BoostServer::BoostServer()
    :iosev_(1)
    , acceptor_(iosev_,
    boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234))
{
}


BoostServer::~BoostServer()
{
    for (auto it = client_mgr_.begin(); it != client_mgr_.end(); ++it)
        delete *it;
    client_mgr_.clear();
}


void BoostServer::RemoveClient(Client* client)
{
    client_mgr_.remove(client);
    delete client;
}


int BoostServer::Run()
{
    while (1)
    {
        boost::asio::ip::tcp::socket* p_sock = new boost::asio::ip::tcp::socket(iosev_);
        acceptor_.accept(*p_sock);
        Client* client = new Client(this, p_sock);
        client_mgr_.push_back(client);
    }
    return 0;
}