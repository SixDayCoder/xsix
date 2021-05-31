#include <iostream>
#include "xsix/network/tcp_connection.hpp"
#include "xsix/network/tcp_server.hpp"

class EchoServer;

class Connection : public xsix::net::TCPConnection
{
public:

	Connection(asio::io_context& ctx, xsix::net::ITCPConnectionParent* parent) : 
		xsix::net::TCPConnection(ctx, parent)
	{}

public:

	virtual void start() override
	{
		async_recv();
	}

protected:

	virtual void handle_error(const asio::error_code& ec) override
	{
		xsix::net::TCPConnection::handle_error(ec);
		if (ec == asio::error::eof ||
			ec == asio::error::connection_reset)
		{
			std::cout << "[Connection] client closed : " << get_id() << std::endl;
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
				std::cout << "[Connection] recv : " << msg << std::endl;
				send(msg.c_str(), msg.length());

				async_recv();
			}
		);
	}

private:

	std::array<char, 1024> m_recv_buffer;	
};
using ConnectionPtr = std::shared_ptr<Connection>;


class EchoServer : public xsix::net::TCPServer<Connection>
{
public:

	EchoServer(asio::io_context& ctx, uint16_t port) :
		xsix::net::TCPServer<Connection>(ctx, port) {}

public:

	virtual void handle_accept_new_connection(ConnectionPtr connptr) override
	{
		xsix::net::TCPServer<Connection>::handle_accept_new_connection(connptr);
		std::cout << "[EchoServer] handle new connection : " << connptr->get_id() << std::endl;
	}

	virtual void handle_remove_connection(ConnectionPtr connptr) override
	{
		xsix::net::TCPServer<Connection>::handle_remove_connection(connptr);
		std::cout << "[EchoServer] close connection : " << connptr->get_id() << std::endl;
	}
};
using EchoServerPtr = std::shared_ptr<EchoServer>;

int main()
{
	asio::io_context ctx;
	EchoServerPtr server(new EchoServer(ctx, 8888));
	server->start();
	server->loop();

	return 0;
}