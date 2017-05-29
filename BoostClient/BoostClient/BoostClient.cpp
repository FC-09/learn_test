#include "BoostClient.h"
#include <stdio.h>
BoostClient::BoostClient()
{
    boost::asio::io_service iosev(1);
    boost::asio::ip::tcp::socket client(iosev);
    client.connect(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::address_v4::from_string("127.0.0.1"), 1234), ec_);

	boost::asio::socket_base::receive_buffer_size option(64 * 1024);
	client.set_option(option);

    Run(client);
}


BoostClient::~BoostClient()
{
}


int BoostClient::Run(boost::asio::ip::tcp::socket &client)
{
    while (1)
    {
        char order;
        std::cout << "请输入指令" << std::endl;
        std::cin >> order;
        client.send(boost::asio::buffer(&order, 1), 0);
        if (order == 'D')
            DownloadFile(client);
        else if (order == 'U')
            UploadFile(client);
        else if (order == 'Q')
            break;
    }
    client.close();
    std::cout << "与服务器连接已断开" << std::endl;
    return 0;
}


int BoostClient::DownloadFile(boost::asio::ip::tcp::socket &client)
{
    std::string s_file_path, c_file_path;
    std::cout << "请输入要下载的远程文件全路径" << std::endl;
    std::cin >> s_file_path;
    std::cout << "请输入下载文件的本地全路径" << std::endl;
    std::cin >> c_file_path;
    client.send(boost::asio::buffer(s_file_path, strlen(s_file_path.c_str())), 0, ec_);
    if (ec_.value() != 0)
        return -1;
    if (FILE* f = fopen(c_file_path.c_str(), "wb"))
    {
		char text_buf[BUF_SIZE + 4] = { 0 };
        char buf[BUF_SIZE + 4] = { 0 };
		int recv_len = 0;
        int old_recv_len = 0;
        while (1)
        {
			recv_len = client.receive(boost::asio::buffer(text_buf, BUF_SIZE + 4), 0, ec_);
            std::cout << recv_len << std::endl;
            if (old_recv_len != 0)
            {
                if (recv_len > (BUF_SIZE + 4 - old_recv_len))
                    memcpy(buf + old_recv_len, text_buf, BUF_SIZE + 4 - old_recv_len);
                else
                    memcpy(buf + old_recv_len, text_buf, recv_len);
            }
            else
            {
                memcpy(buf, text_buf, recv_len);
            } 
            int len = 0;
            memcpy(&len, buf, sizeof(int));
            if (len == (strlen(buf) - 4))
            {
                if (len != 0)
                {
                    int l = fwrite(buf + 4, sizeof(char), len, f);
                    std::cout << l << std::endl;
                    memset(buf, 0, BUF_SIZE + 4);
                    recv_len = 0;
                    old_recv_len = 0;
                }
                else
                {
                    std::cout << "下载完毕" << std::endl;
                    break;
                }
                if (recv_len > (BUF_SIZE + 4 - old_recv_len))
                    memcpy(buf, text_buf + (BUF_SIZE + 4 - old_recv_len), recv_len - (BUF_SIZE + 4 - old_recv_len));
            }
            else
            {
                old_recv_len += recv_len;
            }
            memset(text_buf, 0, BUF_SIZE + 4);
        }
        fclose(f);
    }
    return 0;
}


int BoostClient::UploadFile(boost::asio::ip::tcp::socket &client)
{

    return 0;
}