#include "Boost/asio.hpp"
#include <iostream>
#include <thread>

#define BUF_SIZE   102

class BoostClient
{
public:
    BoostClient();
    ~BoostClient();
    int Run(boost::asio::ip::tcp::socket &client);
    int DownloadFile(boost::asio::ip::tcp::socket &client);
    int UploadFile(boost::asio::ip::tcp::socket &client);
private:
    boost::system::error_code ec_;
    char buf_[BUF_SIZE];
};