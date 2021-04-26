#include "tcp_client.h"
#include <cassert>
#include <ctime>
#include <exception>
#include <iostream>

namespace xsix
{

	TCPClient::TCPClient(asio::io_context& ctx) : m_ctx(ctx), m_tcp_socket(ctx) {}

	void TCPClient::connect(asio::ip::tcp::endpoint ep)
	{
		auto self(shared_from_this());
		asio::error_code ec;
		m_tcp_socket.connect(ep, ec);
		if (ec)
		{
			throw std::runtime_error(ec.message());
		}
		else
		{
			m_tcp_socket.non_blocking(true);
			if (m_connected_handler)
			{
				m_connected_handler(self);
			}
		}
	}

	void TCPClient::async_connect(asio::ip::tcp::endpoint ep)
	{
		auto self(shared_from_this());
		m_tcp_socket.async_connect(ep, [self](const asio::error_code& ec) {
			if (ec)
			{
				throw std::runtime_error(ec.message());
			}
			if (self->m_connected_handler)
			{
				self->m_connected_handler(self);
			}
			}
		);
	}

	void TCPClient::send(const char* msg, std::size_t msgsize)
	{
		if (msg && m_tcp_socket.is_open())
		{
			m_send_buffer.append(msg, msgsize);
		}
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
				if (ec)
				{
					handle_error(ec);
					return;
				}
				m_recv_buffer.append(buf, bytes);
			}
		);
	}

	void TCPClient::async_send()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		std::string msg = m_send_buffer.retrieve_all_as_string();
		m_send_buffer.clear();

		if (msg.size() > 0)
		{
			asio::async_write(m_tcp_socket, asio::buffer(msg),
				[&](const asio::error_code& ec, std::size_t bytes) {
					if (ec)
					{
						handle_error(ec);
						return;
					}
				}
			);
		}
	}

	void TCPClient::recv()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		char bytes[4096] = { 0 };
		asio::error_code ec;
		std::size_t nread = m_tcp_socket.read_some(asio::buffer(bytes), ec);
		if (ec)
		{
			if (ec != asio::error::would_block)
			{
				handle_error(ec);
			}
			return;
		}
		if (nread > 0)
		{
			m_recv_buffer.append(bytes, nread);
		}
	}

	void TCPClient::handle_message()
	{	
		if (m_recv_buffer.empty())
		{
			return;
		}
		if (m_message_handler)
		{
			m_message_handler(shared_from_this());
		}
	}

	void TCPClient::send()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		std::string msg = m_send_buffer.retrieve_all_as_string();
		m_send_buffer.clear();

		if (msg.size() > 0)
		{
			asio::write(m_tcp_socket, asio::buffer(msg));
		}
	}

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

		recv();
		handle_message();
		send();
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
	}
}