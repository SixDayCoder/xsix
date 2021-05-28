#include <stdio.h>
#include <time.h>
#include <thread>
#include <iostream>

#include "xsix/network/tcp_client.hpp"

class EchoClient : public xsix::net::TCPClient
{
public:

	EchoClient(asio::io_context& ctx) : xsix::net::TCPClient(ctx) {}

protected:

	virtual void start() override
	{
		std::string msg = "sixday";
		send(msg.c_str(), msg.length());

		async_recv();
	}

	virtual void handle_connection_established() override
	{
		xsix::net::TCPClient::handle_connection_established();

		std::cout << "[EchoClient] connection established "
			<< " local address : " << local_address()
			<< " remote address : " << remote_address() << std::endl;
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

private:

	void send(const char* msg, int32_t msgsize)
	{
		if (!msg || msgsize < 0)
		{
			return;
		}

		if (!m_tcp_socket.is_open())
		{
			return;
		}

		char buf[4096] = { 0 };
		memcpy(buf, msg, msgsize);
		asio::async_write(m_tcp_socket, asio::buffer(buf, msgsize),
			[&](const asio::error_code& ec, std::size_t bytes) {
				if (ec)
				{
					handle_error(ec);
					return;
				}
			}
		);
	}

	void async_recv()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		m_tcp_socket.async_read_some(asio::buffer(m_recv_buffer),
			[&](const asio::error_code& ec, std::size_t bytes) {
				if (ec)
				{
					handle_error(ec);
					return;
				}

				std::string msg(m_recv_buffer.data(), bytes);
				std::cout << "[EchoClient] recv : " << msg << std::endl;
				send(msg.c_str(), msg.length());

				async_recv();
			}
		);
	}

private:

	std::array<char, 1024> m_recv_buffer;
};
using EchoClientPtr = std::shared_ptr<EchoClient>;

int main()
{
	asio::io_context ctx;
	EchoClientPtr client(new EchoClient(ctx));
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 8888);
	client->async_connect(ep);
	client->loop();
	return 0;
}