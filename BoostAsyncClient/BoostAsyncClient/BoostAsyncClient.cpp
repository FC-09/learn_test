#include "BoostAsyncClient.h"

BoostAsyncClient::BoostAsyncClient()
    :ep_(boost::asio::ip::address::from_string("127.0.0.1"), 1234)
{
    memset(recv_text_, 0, sizeof(recv_text_));
    memset(send_text_, 0, sizeof(send_text_));
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
    /*
    client->async_send(boost::asio::buffer(send_text_, sizeof(send_text_)),
            bind(&BoostAsyncClient::SendHandler, this, client, std::placeholders::_1, std::placeholders::_2));
    client->async_receive(boost::asio::buffer(recv_text_, sizeof(recv_text_)),
        bind(&BoostAsyncClient::RecvHandler, this, client, std::placeholders::_1, std::placeholders::_2));
    */
    Start(client);
    //StartConnect();
}

void BoostAsyncClient::Start(boost::shared_ptr<boost::asio::ip::tcp::socket>& client)
{
    while (1)
    {
        std::cout << "请输入指令：" << std::endl;
        std::cin >> send_text_;
        if (send_text_[0] == 'D')
        {            
            /*std::cout << "请输入下载的远程路径：" << std::endl;
            std::cin >> s_file_path_;            
            std::cout << "请输入下载的本地路径：" << std::endl;
            std::cin >> c_file_path_;*/
            s_file_path_ = { "D:\\000.zip" };
            c_file_path_ = { "D:\\111.zip" };
            int len = s_file_path_.size();
            send_text_[1] = len;
            memcpy(send_text_ + 2, s_file_path_.c_str(), len);
            client->async_send(boost::asio::buffer(send_text_, sizeof(send_text_)),
                bind(&BoostAsyncClient::SendHandler, this, client, std::placeholders::_1, std::placeholders::_2));
        }                
    }
}

void BoostAsyncClient::RecvHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
    boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec)
        return;
    if (f = fopen(c_file_path_.c_str(), "wb"))
    {
        int file_len = 0;
        memcpy(&file_len, recv_text_, sizeof(int));
        if (file_len <= 0)
        {
            fclose(f);
            return;
        }
        fwrite(recv_text_ + 4, sizeof(char), file_len, f);
    }
    client->async_receive(boost::asio::buffer(recv_text_, sizeof(recv_text_)),
        bind(&BoostAsyncClient::RecvHandler, this, client, std::placeholders::_1, std::placeholders::_2));
}

void BoostAsyncClient::SendHandler(boost::shared_ptr<boost::asio::ip::tcp::socket>& client,
    boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec)
        return;
    client->async_receive(boost::asio::buffer(recv_text_, sizeof(send_text_)),
        bind(&BoostAsyncClient::SendHandler, this, client, std::placeholders::_1, std::placeholders::_2));
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