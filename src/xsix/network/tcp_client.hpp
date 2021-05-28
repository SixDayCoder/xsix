#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/buffer.hpp"

#include "tcp_connection.hpp"

#include "asio.hpp"

namespace xsix
{
	namespace net
	{
		class TCPClient : public TCPConnection
		{
		public:

			TCPClient(asio::io_context& ctx) :
				xsix::net::TCPConnection(ctx),
				m_quit(false) {}

			virtual ~TCPClient()
			{
				if (m_tcp_socket.is_open())
				{
					m_tcp_socket.close();
				}
			}

		public:

			void loop()
			{
				m_quit.store(false);
				while (!m_quit.load())
				{
					if (!m_tcp_socket.is_open())
					{
						break;
					}
					auto executed_events = m_io_context.poll();
				}
			}

			void async_connect(asio::ip::tcp::endpoint ep)
			{
				m_tcp_socket.async_connect(ep,
					[this](const asio::error_code& ec) {
						if (ec)
						{
							handle_error(ec);
							return;
						}
						handle_connection_established();
						start();
					}
				);
			}

		protected:

			virtual void handle_connection_established() {}

		protected:

			std::atomic<bool> m_quit;
		};
	}
}
