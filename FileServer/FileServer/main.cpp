#include "boost/asio.hpp"
#include <iostream>
#include "FileServer.h"
#include <thread>
#include <map>

std::map<boost::asio::ip::tcp::socket*, int> m;

int Thread(boost::asio::ip::tcp::socket* sock)
{
	char buf[1024] = { 0 };

	while (1)
	{
		boost::system::error_code ec;
		sock->receive(boost::asio::buffer(buf, sizeof(buf)), 0, ec);
		if (ec.value() != 0)
		{
			m.erase(sock);
			break;
		}

		std::cout << buf << std::endl;
	}

	return 0;
}

int main()
{
    //FileServer().Run();
#if 1
	FileServer file_server;
	file_server.Run();
#else

	boost::asio::io_service iosev(1);
	boost::asio::ip::tcp::acceptor acceptor(iosev,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1234));
	
	while (1)
	{
		boost::asio::ip::tcp::socket* pSock = new boost::asio::ip::tcp::socket(iosev);
		m[pSock] = 0;
		//boost::asio::ip::tcp::socket sock(iosev);
		acceptor.accept(*pSock);
		
		std::thread th(Thread, pSock);
		th.detach();
	}
#endif

    return 0;
}