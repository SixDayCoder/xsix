#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/buffer.h"

#include "asio.hpp"

namespace xsix
{
	class TCPClient;
	using TCPClientPtr = std::shared_ptr<TCPClient>;

	class TCPClient : public xsix::INonCopyable,
					  public std::enable_shared_from_this<TCPClient>
	{
	public:

		using CloseHandler = std::function<void(TCPClientPtr ptr)>;
		using MessageHandler = std::function<void(TCPClientPtr ptr)>;
		using ConnectedHandler = std::function<void(TCPClientPtr ptr)>;

	public:

		TCPClient();

	public:

		void set_close_handler(CloseHandler handler) { m_close_handler = handler; }

		void set_message_handler(MessageHandler handler) { m_message_handler = handler; }

		void set_connected_handler(ConnectedHandler handler) { m_connected_handler = handler; }

	public:

		const std::string local_ip() const { return m_tcp_socket.local_endpoint().address().to_string(); }

		int32_t local_port() const { return (int32_t)(m_tcp_socket.local_endpoint().port()); }

		const std::string remote_ip() const { return m_tcp_socket.remote_endpoint().address().to_string(); }

		int32_t remote_port() const { return (int32_t)(m_tcp_socket.remote_endpoint().port()); }

	public:

		void async_connect(asio::ip::tcp::endpoint ep);

		void send(const char* msg, std::size_t msgsize);

		void loop();

		void tick();

		asio::io_context& get_context() { return m_ctx; }

	private:

		void start();

		void async_recv();

		void handle_message();

		void handle_error(const asio::error_code& ec);

	public:

		xsix::buffer			m_send_buffer;
		xsix::buffer			m_recv_buffer;

	private:

		asio::io_context	    m_ctx;
		asio::ip::tcp::socket   m_tcp_socket;

		CloseHandler			m_close_handler;
		MessageHandler			m_message_handler;
		ConnectedHandler		m_connected_handler;
	};
}
