#include "tcp_conn.h"

namespace xsix
{
	void TCPConn::send(const char* msg, std::size_t msgsize)
	{
		if (msg && m_tcp_socket.is_open())
		{
			m_send_buffer.append(msg, msgsize);
		}
	}

	void TCPConn::tick()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		//https://www.boost.org/doc/libs/1_36_0/doc/html/boost_asio/example/echo/async_tcp_echo_server.cpp
		async_recv();
		default_msg_process(m_recv_buffer);
		async_send();
	}

	void TCPConn::default_msg_process(xsix::buffer& msgbuffer)
	{
		std::string msg = msgbuffer.retrieve_all_as_string();
		msgbuffer.clear();
		if (msg.size() > 0)
		{
			printf("tcp conn id : %d, recv : %s\n", get_id(), msg.c_str());

			//TODO: echo test
			send(msg.c_str(), msg.size());
		}
	}

	void TCPConn::handle_disconnect()
	{
		printf("client closed!\n");
		m_tcp_socket.close();
		if (m_close_handler)
		{
			m_close_handler(shared_from_this());
		}
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
			printf("handle error : %s\n", ec.message().c_str());
			m_tcp_socket.close();
			if (m_close_handler)
			{
				m_close_handler(shared_from_this());
			}
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
			}
		);
	}

	void TCPConn::async_send()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		std::string msg = m_send_buffer.retrieve_all_as_string();
		m_send_buffer.clear();

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