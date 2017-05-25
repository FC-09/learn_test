#include "BoostClient.h"

BoostClient::BoostClient()
{
    memset(buf_, 0, BUF_SIZE);
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
        memset(buf_, 0, BUF_SIZE);
        std::cout << "请输入指令" << std::endl;
        std::cin >> buf_;
        /*boost::system::error_code ec;
        client.send(boost::asio::buffer(buf_, sizeof("hello world")), 0, ec);
        if (ec.value() != 0)
            return -1;*/
        if (buf_[0] = 'D')
        {
            /*std::thread thread_download(&BoostClient::DownloadFile, this, client);
            thread_download.detach();*/
            DownloadFile(client);
        }
        else if (buf_[0] = 'U')
        {
            /*std::thread thread_upload(&BoostClient::DownloadFile, this, client);
            thread_upload.detach();*/
            UploadFile(client);
        }
        else if (buf_[0] = 'Q')
        {
            client.send(boost::asio::buffer(buf_, BUF_SIZE), 0);
            break;
        } 
    }
    client.close();
    std::cout << "与服务器连接已断开" << std::endl;
    return 0;
}


int BoostClient::DownloadFile(boost::asio::ip::tcp::socket &client)
{
    char s_file_path[100] = { 0 };
    char c_file_path[100] = { 0 };
    std::cout << "请输入要下载的远程文件全路径" << std::endl;
    std::cin >> s_file_path;
    std::cout << "请输入下载文件的本地全路径" << std::endl;
    std::cin >> c_file_path;
    char len_path = strlen(s_file_path);
    buf_[1] = len_path;
    strcpy(buf_ + 2, s_file_path);
    client.send(boost::asio::buffer(buf_, len_path + 2), 0, ec_);
    if (ec_.value() != 0)
        return -1;
    FILE* f = fopen(c_file_path, "wb");;
    while (1)
    {
        char text_buf[100] = { 0 };
        memset(buf_, 0, BUF_SIZE);
        client.receive(boost::asio::buffer(buf_, BUF_SIZE), 0, ec_);
        int len = buf_[0];
        strcpy(text_buf, buf_ + 1);
        if (text_buf == "ok")
        {
            std::cout << "下载完毕" << std::endl;
            break;
        }
        fwrite(text_buf, sizeof(char), len, f);
    }
    fclose(f);
    return 0;
}


int BoostClient::UploadFile(boost::asio::ip::tcp::socket &client)
{

    return 0;
}