#include "Client.h"
#include "BoostAsyncServer.h"

Client::Client(BoostAsyncServer* server, boost::shared_ptr<boost::asio::ip::tcp::socket>& p_sock)
    : p_client_(p_sock)
    , p_server_(server)
    , read_len_(0)
{
    memset(recv_text_, 0, BUF_SIZE);
    memset(send_text_, 0, BUF_SIZE);
    memset(file_path_, 0, sizeof(file_path_));
    std::cout << "Client()" << std::endl;
}

Client::~Client()
{
    std::cout << "~Client()" << std::endl;
}

void Client::Run()
{
    p_client_->async_receive(boost::asio::buffer(recv_text_, sizeof(recv_text_)),
        std::bind(&Client::RecvHandler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Client::RecvHandler(boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec)
    {
        //p_server_->RemoveClient(this);
        std::cout << "连接中断" << std::endl;
        return;
    }
    else
    {
        std::cout << "接收成功" << bytes_transferred << std::endl;
        if (recv_text_[0] == 'D')
        {
            memcpy(file_path_, recv_text_ + 2, recv_text_[1]);
            if (f = fopen(file_path_, "rb"))
            {
                FileDownload();
                p_client_->async_send(boost::asio::buffer(send_text_, read_len_ + 4),
                    std::bind(&Client::SendHandler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            }
        }
    }
}

void Client::SendHandler(boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec)
    {
        std::cout << "连接中断" << std::endl;
    }
    else
    {
        std::cout << "发送成功" << bytes_transferred << std::endl;
        if (read_len_ <= 0)
            Run();
        else
        {
            FileDownload();
            p_client_->async_send(boost::asio::buffer(send_text_, read_len_ + 4),
                std::bind(&Client::SendHandler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
        }
    }
}

void Client::FileDownload()
{
    if (f != 0)
    {
        read_len_ = fread(send_text_ + 4, sizeof(char), BUF_SIZE - 4, f);
        if (read_len_ <= 0)
            fclose(f);
        std::cout << "读取文件:" << read_len_ << std::endl;
        memcpy(send_text_, &read_len_, sizeof(int));
    }
}