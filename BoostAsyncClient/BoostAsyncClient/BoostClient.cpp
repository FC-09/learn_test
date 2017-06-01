
#if 0

#include "Boost/asio.hpp"
#include <iostream>

class BoostClient
{
public:
    BoostClient()
    {
        boost::asio::io_service iosev(1);
        boost::asio::ip::tcp::socket client(iosev);
        client.connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address_v4::from_string("127.0.0.1"), 1234), ec_);
        Run(client);
    }

    ~BoostClient(){}

    int Run(boost::asio::ip::tcp::socket &client)
    {
        char text[20] = { 0 };
        int l = client.receive(boost::asio::buffer(text, sizeof(text)), 0);
        std::cout << "已接收数据：";
        for (int i = 0; i < l; ++i)
            std::cout << text[i];
        std::cout << std::endl;
        client.close();
        std::cout << "与服务器连接已断开" << std::endl;
        system("pause");
        return 0;
    }

private:
    boost::system::error_code ec_;
};

int main()
{
    BoostClient client;
    return 0;
}

#endif