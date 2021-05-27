#include <iostream>
#include "xsix/network/tcp_connection.hpp"
#include "xsix/network/tcp_server.hpp"

class Connection : public xsix::net::TCPConnection
{
public:

	Connection(asio::io_context& ctx) : xsix::net::TCPConnection(ctx) {}

protected:

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
			std::cout << "[Connection] recv : " << std::string(buf, size) << std::endl;
			send(buf, size);
		}
	}

	virtual void handle_error(const asio::error_code& ec) override
	{
		xsix::net::TCPConnection::handle_error(ec);
		if (ec == asio::error::eof ||
			ec == asio::error::connection_reset)
		{
			std::cout << "[Connection] client closed : " << get_id() << std::endl;
		}
	}
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

	virtual void handle_close_connection(ConnectionPtr connptr, const asio::error_code& ec) override
	{
		xsix::net::TCPServer<Connection>::handle_close_connection(connptr, ec);
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