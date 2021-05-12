#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/buffer.h"

#include "asio.hpp"

#include <memory>
#include <functional>

namespace xsix
{
	class TCPConn;

	using TCPConnPtr = std::shared_ptr<TCPConn>;

	class TCPConn : public INonCopyable, 
					public std::enable_shared_from_this<TCPConn>
	{
	public:

		using CloseHandler = std::function<void(TCPConnPtr ptr, const asio::error_code& ec)>;

		using MessageHandler = std::function<void(TCPConnPtr ptr)>;

	public:

		enum
		{
			STATE_INVALID = -1,       //初始状态
			STATE_CONNECTING = 1,     //连接中
			STATE_CONNECTED = 2,      //已连接
			STATE_DISCONNECTING = 3,  //断开中
			STATE_DISCONNECTED = 4,   //已断开
		};

	public:

		TCPConn(asio::io_context& ctx) : m_tcp_socket(ctx) {}

	public:

		asio::ip::tcp::socket& socket() { return m_tcp_socket; }

		const std::string local_ip() const { return m_tcp_socket.local_endpoint().address().to_string(); }

		int32_t local_port() const { return (int32_t)(m_tcp_socket.local_endpoint().port()); }

		const std::string remote_ip() const { return m_tcp_socket.remote_endpoint().address().to_string(); }

		int32_t remote_port() const { return (int32_t)(m_tcp_socket.remote_endpoint().port()); }

		void    set_id(int32_t id) { m_id = id; }

		int32_t get_id() const { return m_id; }

		void set_state(int32_t state) { m_state = state; }

		int32_t get_state() const { return m_state; }

	public:

		void start();

		void tick();

		void send(const char* msg, std::size_t msgsize) 
		{
			if (msg && m_tcp_socket.is_open())
			{
				m_send_buffer.append(msg, msgsize);
			}
		}

		void set_close_handler(CloseHandler handler) { m_close_handler = handler; }

		void set_message_handler(MessageHandler handler) { m_message_handler = handler; }

	private:

		void handle_disconnect();

		void handle_error(const asio::error_code& ec);

	private:

		void handle_message();

		void async_recv();

		void async_send();

	public:

		xsix::buffer		  m_recv_buffer;
		xsix::buffer		  m_send_buffer;

	private:

		int32_t				  m_id = -1;
		int32_t				  m_state = STATE_INVALID;
		asio::ip::tcp::socket m_tcp_socket;

		CloseHandler		  m_close_handler;
		MessageHandler		  m_message_handler;
	};
}