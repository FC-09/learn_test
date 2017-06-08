#include "BoostAsyncClient.h"

BoostAsyncClient::BoostAsyncClient()
    : ep_(boost::asio::ip::address::from_string("127.0.0.1"), 1234)
    , client_(new boost::asio::ip::tcp::socket(ios_))
    , recv_text_len_(0)
    , end_download_(0)
{
    memset(recv_text_, 0, sizeof(recv_text_));
    memset(send_text_, 0, sizeof(send_text_));
    std::cout << "BoostAsyncClient()" << std::endl;
}
BoostAsyncClient::~BoostAsyncClient()
{
    std::cout << "~BoostAsyncClient()" << std::endl;
}

int BoostAsyncClient::Run()
{
    StartConnect();
    return ios_.run();
}

void BoostAsyncClient::StartConnect()
{
    client_->async_connect(ep_, std::bind(&BoostAsyncClient::ConnectHandler, this, std::placeholders::_1));
}

void BoostAsyncClient::ConnectHandler(boost::system::error_code ec)
{
    if (ec)
        return;
    /*std::cout << "recive from:" << client->remote_endpoint().address() << std::endl;;
    std::cout << " port:" << client->remote_endpoint().port() << std::endl;*/
    Start();
    //StartConnect();
}

void BoostAsyncClient::Start()
{
    std::cout << "������ָ�" << std::endl;
    std::cin >> send_text_;
    if (send_text_[0] == 'D')
    {
        std::cout << "���������ص�Զ��·����" << std::endl;
        std::cin >> s_file_path_;
        std::cout << "���������صı���·����" << std::endl;
        std::cin >> c_file_path_;
        /*s_file_path_ = { "D:\\000.zip" };
        c_file_path_ = { "D:\\111.zip" };*/
        int len = s_file_path_.size();
        send_text_[1] = len;
        memcpy(send_text_ + 2, s_file_path_.c_str(), len);
        if (f = fopen(c_file_path_.c_str(), "wb"))
        {
            recv_text_len_ = 0;
            client_->async_send(boost::asio::buffer(send_text_, len + 2),
                bind(&BoostAsyncClient::SendHandler, this, std::placeholders::_1, std::placeholders::_2));
            std::cout << "�������أ���ȴ��������ж����أ�������1" << std::endl;
            end_ = std::thread(&BoostAsyncClient::EndDownload,this);
            end_.detach();
        }
        else
        {
            std::cout << "·�����������¿�ʼ" << std::endl;
            Start();
        }
    }
}

void BoostAsyncClient::SendHandler(boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec || end_download_)
    {
        std::cout << "���ӶϿ�" << std::endl;
        return;
    }
    client_->async_receive(boost::asio::buffer(recv_text_, sizeof(send_text_)),
        bind(&BoostAsyncClient::RecvHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void BoostAsyncClient::RecvHandler(boost::system::error_code ec, size_t bytes_transferred)
{
    if (ec || end_download_)
    {
        std::cout << "���ӶϿ�" << std::endl;
        return;
    }
    if (WriteFile(bytes_transferred))
    {
        //_endthreadex(end_.get_id);
        end_.~thread();
        Start();
    }
        
    else
    {
        //std::cout << "����ǰ��������" << recv_text_len_ << std::endl;
        client_->async_receive(boost::asio::buffer(recv_text_ + recv_text_len_, sizeof(recv_text_)),
            bind(&BoostAsyncClient::RecvHandler, this, std::placeholders::_1, std::placeholders::_2));
    }
}

int BoostAsyncClient::WriteFile(int bytes_transferred)
{
    int recv_file_len = 0;
    recv_text_len_ += bytes_transferred;
    //std::cout << "���պ󻺴�����" << recv_text_len_ << std::endl;
    memcpy(&recv_file_len, recv_text_, sizeof(int));
    while (recv_text_len_ >= recv_file_len + 4)
    {
        if (recv_file_len > 0)
        {
            int l = fwrite(recv_text_ + 4, sizeof(char), recv_file_len, f);
            //std::cout << "д�����ݣ�" << l << std::endl;
            memcpy(recv_text_, recv_text_ + recv_file_len + 4, recv_text_len_ - recv_file_len - 4);
            recv_text_len_ -= (recv_file_len + 4);
            memcpy(&recv_file_len, recv_text_, sizeof(int));
        }
        else
            break;
    }
    if (recv_file_len <= 0)
    {
        fclose(f);
        std::cout << "�������" << std::endl;
        return 1;
    }
    return 0;
}

void BoostAsyncClient::EndDownload()
{ 
    std::cin >> end_download_;
}