#include "Boost/asio.hpp"
#include <iostream>
#include <string>
#define  BUF_SIZE  30*1024

class BoostAsyncClient
{
public:
    BoostAsyncClient();
        
    ~BoostAsyncClient();

    int Run();

    void StartConnect();

    void ConnectHandler(boost::system::error_code ec);

    void RecvHandler(boost::system::error_code ec, size_t bytes_transferred);    

    void SendHandler(boost::system::error_code ec, size_t bytes_transferred);

    void Start();

    int WriteFile(int);    

    //int ReadFile();
    
private:
    boost::asio::io_service ios_;
    boost::asio::ip::tcp::endpoint ep_;
    boost::shared_ptr<boost::asio::ip::tcp::socket> client_;
    char recv_text_[2*BUF_SIZE];
    char send_text_[BUF_SIZE];
    std::string s_file_path_;
    std::string c_file_path_;
    int recv_text_len_;
    FILE* f;
};