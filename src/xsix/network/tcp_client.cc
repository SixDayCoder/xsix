#include "tcp_client.h"
#include <cassert>
#include <ctime>
#include <exception>
#include <iostream>

namespace xsix
{

	TCPClient::TCPClient() : 
		m_tcp_socket(m_ctx), 
		m_recv_buffer(), 
		m_send_buffer() {}

	void TCPClient::loop()
	{
		while (m_tcp_socket.is_open())
		{
			tick();
		}
	}

	void TCPClient::tick()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}
		auto executed_events = m_ctx.poll();
	}

	void TCPClient::async_connect(asio::ip::tcp::endpoint ep)
	{
		auto self(shared_from_this());
		m_tcp_socket.async_connect(ep,
			[self](const asio::error_code& ec) {
				if (ec)
				{
					self->handle_error(ec);
					return;
				}
				if (self->m_connected_handler)
				{
					self->m_connected_handler(self);
				}
				self->start();
			}
		);
	}

	void TCPClient::start()
	{
		async_recv();
	}

	void TCPClient::send(const char* msg, std::size_t msgsize)
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

				std::cout << "----- client write bytes : " << bytes << std::endl;

				if (ec)
				{
					handle_error(ec);
					return;
				}
			}
		);
	}

	void TCPClient::async_recv()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		char buf[4096] = { 0 };
		m_tcp_socket.async_read_some(asio::buffer(buf),
			[&](const asio::error_code& ec, std::size_t bytes) {

				std::cout << "+++++ client recv bytes : " << bytes << std::endl;

				if (ec)
				{
					handle_error(ec);
					return;
				}

				m_recv_buffer.append(buf, bytes);

				handle_message();
				
				//fire another event
				async_recv();
			}
		);
	}

	void TCPClient::handle_message()
	{
		if (m_message_handler)
		{
			m_message_handler(shared_from_this());
		}
	}

	void TCPClient::handle_error(const asio::error_code& ec)
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		if (ec == asio::error::eof ||
			ec == asio::error::connection_reset)
		{
			printf("server closed error : %s\n", ec.message().c_str());
		}
		else
		{
			printf("tcp client error : %s\n", ec.message().c_str());
		}
		m_tcp_socket.close();
		if (m_close_handler)
		{
			m_close_handler(shared_from_this());
		}
	}
}