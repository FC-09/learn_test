#include "BoostClient.h"
#include <stdio.h>
BoostClient::BoostClient()
{
    //memset(buf_, 0, BUF_SIZE);
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
        //memset(buf_, 0, BUF_SIZE);
        char order;
        std::cout << "请输入指令" << std::endl;
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
		int recv_len = 0;
        while (1)
        {
			recv_len = client.receive(boost::asio::buffer(text_buf + recv_len, BUF_SIZE + 4), 0, ec_);
			static int recvCount = 0;
			recvCount++;
			static int totalRecvLength = 0;
			totalRecvLength += recv_len;
			if (20 == recvCount)
				int k = 0;

			printf("recv, recvLength = %d, %d, totalRecvLength = %d\n", recv_len, recvCount, totalRecvLength);

            std::cout << recv_len << std::endl;
            int len = 0;
            memcpy(&len, text_buf, sizeof(int));
			if (len < (recv_len - 4))
			{
				memcpy(&len, text_buf + len + 4, sizeof(int));
			}
            else if (len != (recv_len - 4))
                continue;
            if (len == 0)
            {
                std::cout << "下载完毕" << std::endl;
                break;
            }
            int l = fwrite(text_buf + 4, sizeof(char), len, f);
			static int totalWriteLength = 0;
			totalWriteLength += l;
			printf("=================, write file ,len = %d, length = %d, total = %d\n", len, l, totalWriteLength);
            //std::cout << l << std::endl;
			memset(text_buf, 0, BUF_SIZE + 4);
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