#include "Boost/asio.hpp"
#include <iostream>

class BoostAsyncClient
{
public:
    BoostAsyncClient();
        
    ~BoostAsyncClient();

    int Run();

    void StartConnect();

    void ConnectHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client, boost::system::error_code ec);

    void ReadHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
        boost::system::error_code ec, size_t bytes_transferred);    

    int WriteFile();    

    int ReadFile();
    
private:
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::endpoint ep_;
    char recv_text_[30 * 1024];
};