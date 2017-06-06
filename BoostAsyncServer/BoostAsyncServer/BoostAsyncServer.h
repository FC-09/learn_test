#include "Client.h"
#include "boost/asio.hpp" 
#include "boost/shared_ptr.hpp"
#include <list>


class BoostAsyncServer
{
public:
    BoostAsyncServer();        

    ~BoostAsyncServer();

    int Run();
    
    void StartAccept();
    
    void AcceptHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client, boost::system::error_code ec);
    
    /*void SendHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
        boost::system::error_code ec, size_t bytes_transferred);

    void RecvHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
        boost::system::error_code ec, size_t bytes_transferred);*/
    
    void RemoveClient(boost::shared_ptr<boost::asio::ip::tcp::socket>&);

private:
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::acceptor acc_;    
    std::list<boost::shared_ptr<boost::asio::ip::tcp::socket>> clients_;
};