#pragma once

#include "tcp_conn.h"
#include "tcp_conn_manager.h"

#include "asio.hpp"

#include <atomic>
#include <memory>
#include <map>
#include <functional>

namespace xsix
{
	class TCPServer
	{
	public:

		using AcceptFuncHandler = std::function<void(TCPConnPtr, const asio::error_code& ec)>;

	public:

		TCPServer(asio::io_context& ctx, uint16_t port);

	public:

		void set_reuse_addr(bool reuse);

		void set_accept_handler(AcceptFuncHandler handler) { m_accept_handler = handler; }

		void accept();

		void loop();

		void tick();

	private:

		void default_accept_handler(TCPConnPtr connptr, const asio::error_code& ec);

		void on_conn_close(TCPConnPtr ptr);

	private:

		asio::io_context&       m_ctx;
		asio::ip::tcp::endpoint m_listen_ep;
		asio::ip::tcp::acceptor m_tcp_acceptor;
		xsix::TCPConnManager	m_tcp_conn_mgr;

		AcceptFuncHandler		m_accept_handler;
	};

}

