#include "tcp_conn.h"
#include "tcp_conn_manager.h"
#include <iostream>

namespace xsix
{

	void TCPConn::start()
	{
		async_recv();
	}

	void TCPConn::tick()
	{

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

				std::cout << "+++++ conn id : " << get_id() << " recv bytes : " << bytes << std::endl;

				if (ec)
				{
					handle_error(ec);
					return;
				}

				XASSERT(m_recv_buffer.append(buf, bytes) > 0);

				handle_message();

				//fire another event
				async_recv();
			}
		);
	}


	void TCPConn::send(const char* msg, int32_t msgsize)
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

				std::cout << "----- conn id : " << get_id() << " write bytes : " << bytes << std::endl;

				if (ec)
				{
					handle_error(ec);
					return;
				}
			}
		);
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

	void TCPConn::handle_message()
	{
		if (m_message_handler)
		{
			m_message_handler(shared_from_this());
		}
	}
}