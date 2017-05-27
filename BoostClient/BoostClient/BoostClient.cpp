#include "BoostClient.h"

BoostClient::BoostClient()
{
    //memset(buf_, 0, BUF_SIZE);
    boost::asio::io_service iosev(1);
    boost::asio::ip::tcp::socket client(iosev);
    client.connect(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address_v4::from_string("127.0.0.1"), 1234), ec_);
    
    Run(client);
}


BoostClient::~BoostClient()
{
}


int BoostClient::Run(boost::asio::ip::tcp::socket &client)
{
    while (1)
    {
        //memset(buf_, 0, BUF_SIZE);
        char order;
        std::cout << "������ָ��" << std::endl;
        std::cin >> order;
        client.send(boost::asio::buffer(&order, 1), 0);
        /*boost::system::error_code ec;
        client.send(boost::asio::buffer(buf_, sizeof("hello world")), 0, ec);
        if (ec.value() != 0)
            return -1;*/
        if (order == 'D')
        {
            /*std::thread thread_download(&BoostClient::DownloadFile, this, client);
            thread_download.detach();*/
            DownloadFile(client);
        }
        else if (order == 'U')
        {
            /*std::thread thread_upload(&BoostClient::DownloadFile, this, client);
            thread_upload.detach();*/
            
            UploadFile(client);
        }
        else if (order == 'Q')
        {
            
            break;
        } 
    }
    client.close();
    std::cout << "������������ѶϿ�" << std::endl;
    return 0;
}


int BoostClient::DownloadFile(boost::asio::ip::tcp::socket &client)
{
    std::string s_file_path, c_file_path;
    std::cout << "������Ҫ���ص�Զ���ļ�ȫ·��" << std::endl;
    std::cin >> s_file_path;
    std::cout << "�����������ļ��ı���ȫ·��" << std::endl;
    std::cin >> c_file_path;
    client.send(boost::asio::buffer(s_file_path, strlen(s_file_path.c_str())), 0, ec_);
    if (ec_.value() != 0)
        return -1;
    if (FILE* f = fopen(c_file_path.c_str(), "wb"))
    {
        char text_buf[63 * 1024 + 4] = { 0 };
        int recv_len = 0;
        while (1)
        {
            recv_len = client.receive(boost::asio::buffer(text_buf + recv_len, 63 * 1024 + 4), 0, ec_);
            std::cout << recv_len << std::endl;
            int len = 0;
            memcpy(&len, text_buf, sizeof(int));
            if (len != (recv_len - 4))
                continue;
            else if (len == 0)
            {
                std::cout << "�������" << std::endl;
                break;
            }
            int l = fwrite(text_buf + 4, sizeof(char), len, f);
            std::cout << l << std::endl;
            memset(text_buf, 0, 63 * 1024 + 4);
            recv_len = 0;
        }
        fclose(f);
    }
    return 0;
}


int BoostClient::UploadFile(boost::asio::ip::tcp::socket &client)
{

    return 0;
}