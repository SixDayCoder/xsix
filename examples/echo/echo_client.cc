#include <stdio.h>
#include <time.h>
#include <thread>
#include <iostream>

#include "xsix/network/tcp_client.hpp"

class EchoClient : public xsix::net::TCPClient
{
public:

	EchoClient() : xsix::net::TCPClient() {}

protected:

	virtual void handle_connection_established() override
	{
		xsix::net::TCPClient::handle_connection_established();

		std::cout << "[EchoClient] connection established "
			<< " local address : " << local_address()
			<< " remote address : " << remote_address() << std::endl;
		std::string msg = "sixday";
		send(msg.c_str(), msg.length());
	}

	virtual void handle_message() override
	{
		if (m_recv_buffer.empty())
		{
			return;
		}

		char buf[4096] = { 0 };
		int32_t size = m_recv_buffer.write_to(buf, 4096);
		if (size > 0)
		{
			std::cout << "[EchoClient] recv : " << std::string(buf, size) << std::endl;
			send(buf, size);
		}
	}

	virtual void handle_error(const asio::error_code& ec) override
	{
		xsix::net::TCPClient::handle_error(ec);

		if (ec == asio::error::eof ||
			ec == asio::error::connection_reset)
		{
			std::cout << "[EchoClient] server closed : " << ec.message() << std::endl;
		}
		else
		{
			std::cout << "[EchoClient] client error : " << ec.message() << std::endl;
		}
	}

};
using EchoClientPtr = std::shared_ptr<EchoClient>;

int main()
{
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 8888);
	EchoClientPtr client(new EchoClient());
	client->async_connect(ep);
	client->loop();
	return 0;
}