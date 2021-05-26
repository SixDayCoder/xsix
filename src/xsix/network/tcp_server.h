#pragma once

#include "tcp_conn_manager.h"

#include <atomic>
#include <memory>
#include <map>
#include <functional>

namespace xsix
{
	class TCPServer;
	using TCPServerPtr = std::shared_ptr<TCPServer>;

	class TCPServer : public xsix::INonCopyable,
					  public std::enable_shared_from_this<TCPServer>
	{
	public:

		using AcceptHandler = std::function<void(TCPServerPtr server, TCPConnPtr conn, const asio::error_code& ec)>;
		using ConnMessageHandler = std::function<void(TCPConnPtr ptr)>;
		using ConnCloseHandler = std::function<void(TCPConnPtr ptr, const asio::error_code& ec)>;

	public:

		TCPServer(asio::io_context& ctx, uint16_t port);

	public:

		void set_reuse_addr(bool reuse);

		void accept();

		void loop();

		void tick();

		void broadcast(const char* msg, int32_t size);

		TCPConnPtr get_conn_ptr(int32_t connid);

	public:

		void set_accept_handler(AcceptHandler handler) { m_accept_handler = handler; }

		void set_conn_message_handler(ConnMessageHandler handler) { m_conn_message_handler = handler; }

		void set_conn_close_handler(ConnCloseHandler handler) { m_conn_close_handler = handler; }
		
	public:

		asio::io_context& ctx() { return m_ctx; }

	private:

		asio::io_context&       m_ctx;
		asio::ip::tcp::endpoint m_listen_ep;
		asio::ip::tcp::acceptor m_tcp_acceptor;
		xsix::TCPConnManager	m_tcp_conn_mgr;

		AcceptHandler			m_accept_handler;
		ConnMessageHandler	    m_conn_message_handler;
		ConnCloseHandler		m_conn_close_handler;
	};

}

