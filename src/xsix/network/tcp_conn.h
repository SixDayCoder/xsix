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

		using OnCloseHandler = std::function<void(TCPConnPtr pt)>;

	public:

		TCPConn(asio::io_context& ctx) : m_tcp_socket(ctx) {}

	public:

		asio::ip::tcp::socket& socket() { return m_tcp_socket; }

		const std::string ip() const { return m_tcp_socket.remote_endpoint().address().to_string(); }

		int32_t port() const { return (int32_t)m_tcp_socket.remote_endpoint().port(); }

		void    set_id(int32_t id) { m_id = id; }

		int32_t get_id() const { return m_id; }

	public:

		void send(const char* msg, std::size_t msgsize);

		void tick();

		void set_on_close(OnCloseHandler onclose) { m_close_handler = onclose; }

	private:

		void default_msg_process(xsix::buffer& msgbuffer);

		void handle_disconnect();

		void handle_error(const asio::error_code& ec);

	private:

		void async_recv();

		void async_send();

	private:

		int32_t				  m_id = -1;
		asio::ip::tcp::socket m_tcp_socket;

		xsix::buffer		  m_recv_buffer;
		xsix::buffer		  m_send_buffer;

		OnCloseHandler		  m_close_handler;

	};
}