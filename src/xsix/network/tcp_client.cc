#include "tcp_client.h"

namespace xsix
{

	TCPClient::TCPClient(asio::io_context& ctx) : m_ctx(ctx), m_tcp_socket(ctx) {}

	void TCPClient::connect(asio::ip::tcp::endpoint ep)
	{
		m_tcp_socket.connect(ep);
	}

	void TCPClient::send(const char* msg, std::size_t msgsize)
	{
		if (msg && m_tcp_socket.is_open())
		{
			m_send_buffer.append(msg, msgsize);
		}
	}

	void TCPClient::recv()
	{
		if (!m_tcp_socket.is_open())
		{
			return;
		}

		asio::error_code ec;
		std::size_t can_read = m_tcp_socket.available(ec);
		if (ec)
		{
			handle_error(ec);
			return;
		}

		if (can_read > 0)
		{
			char bytes[4096] = { 0 };
			std::size_t nread = m_tcp_socket.read_some(asio::buffer(bytes), ec);	
			if (ec)
			{
				handle_error(ec);
				return;
			}
			m_recv_buffer.append(bytes, nread);
		}
	}

	void TCPClient::send_buffered_msg()
	{
		if (m_tcp_socket.is_open())
		{
			std::string msg = m_send_buffer.retrieve_all_as_string();
			m_send_buffer.clear();

			asio::error_code ec;
			m_tcp_socket.write_some(asio::buffer(msg), ec);
			if (ec)
			{
				handle_error(ec);
				return;
			}
		}
	}

	void TCPClient::loop()
	{
		while (m_tcp_socket.is_open())
		{
			//TODO:echo test
			auto ts = std::chrono::system_clock::now().time_since_epoch().count() / 10000;
			std::string msg = "echo : " + std::to_string(ts);
			send(msg.c_str(), msg.size());

			tick();

			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		}
		printf("tcp::client quit loop\n");
	}

	void TCPClient::tick()
	{
		if (m_tcp_socket.is_open())
		{
			recv();
			if (m_msg_processor)
			{
				m_msg_processor(m_recv_buffer);
			}
			else
			{
				default_msg_process(m_recv_buffer);
			}
			send_buffered_msg();
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
			m_tcp_socket.close();
		}
	}

	void TCPClient::default_msg_process(xsix::buffer& msgbuffer)
	{
		std::string msg = msgbuffer.retrieve_all_as_string();
		msgbuffer.clear();
		if (msg.size() > 0)
		{
			printf("tcp client recv : %s\n", msg.c_str());
		}
	}
}