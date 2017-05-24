#include "Client.h"
#include "boost/asio.hpp"
#include <thread>
#include <list>

class BoostServer
{
public:
    BoostServer();
    ~BoostServer();
    int Run();
    void RemoveClient(Client* client);

private:
    std::list<Client*> client_mgr_;
    boost::asio::ip::tcp::socket* sock_;
    boost::asio::io_service iosev;
    boost::asio::ip::tcp::acceptor acceptor;
};