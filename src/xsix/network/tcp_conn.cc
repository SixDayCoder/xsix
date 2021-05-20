#include "tcp_conn.h"
#include <iostream>

namespace xsix
{

	void TCPConn::start()
	{
		async_recv();
	}

	void TCPConn::tick()
	{
		handle_message();
		async_send();
	}

	void TCPConn::send(const char* msg, int32_t msgsize)
	{
		if (msg && m_tcp_socket.is_open())
		{
			m_send_buffer.append(msg, msgsize);
		}
	}

	void TCPConn::handle_disconnect()
	{
		auto self(shared_from_this());
		set_state(STATE_DISCONNECTING);
		if (m_close_handler)
		{
			m_close_handler(self, asio::error::connection_reset);
		}
		set_state(STATE_DISCONNECTED);
		m_tcp_socket.close();
	}

	void TCPConn::handle_error(const asio::error_code& ec)
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		if (ec == asio::error::eof ||
			ec == asio::error::connection_reset)
		{
			handle_disconnect();
		}
		else
		{
			auto self(shared_from_this());
			if (m_close_handler)
			{
				m_close_handler(self, ec);
			}
			m_tcp_socket.close();
		}
	}

	void TCPConn::async_recv()
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

				//fire another event
				async_recv();
			}
		);
	}

	void TCPConn::handle_message()
	{
		if (m_message_handler)
		{
			m_message_handler(shared_from_this());
		}
	}

	void TCPConn::async_send()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		if (m_send_buffer.empty())
		{
			return;
		}

		char buf[4096] = { 0 };
		int32_t writesize = m_send_buffer.write_to(buf, 4096);
		if (writesize > 0)
		{
			asio::async_write(m_tcp_socket, asio::buffer(buf, writesize),
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
}