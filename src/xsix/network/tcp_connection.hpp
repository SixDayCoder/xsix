#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/buffer.hpp"

#include "tcp_base_interface.hpp"

#include "asio.hpp"

namespace xsix
{
	namespace net
	{
		class TCPConnection : public INonCopyable
		{
		public:

			TCPConnection(asio::io_context& ctx, ITCPConnectionParent* parent) :
				m_tcp_conenction_parent(parent),
				m_io_context(ctx),
				m_tcp_socket(ctx)
			{}

			virtual ~TCPConnection() {}

		public:

			asio::ip::tcp::socket& socket() { return m_tcp_socket; }
			asio::io_context& get_context() { return m_io_context; }
			bool is_valid() const { return m_tcp_socket.is_open(); }

		public:

			const std::string local_ip() const { return m_tcp_socket.local_endpoint().address().to_string(); }
			const std::string remote_ip() const { return m_tcp_socket.remote_endpoint().address().to_string(); }

			int32_t local_port() const { return (int32_t)(m_tcp_socket.local_endpoint().port()); }
			int32_t remote_port() const { return (int32_t)(m_tcp_socket.remote_endpoint().port()); }

			std::string local_address() const { return "(" + local_ip() + ":" + std::to_string(local_port()) + ")"; }
			std::string remote_address() const { return "(" + remote_ip() + ":" + std::to_string(remote_port()) + ")"; }

			void    set_id(int32_t id) { m_id = id; }
			int32_t get_id() const { return m_id; }

		public:

			void broadcast(const char* msg, std::size_t msgsize)
			{
				if (m_tcp_conenction_parent)
				{
					m_tcp_conenction_parent->broadcast(msg, msgsize);
				}		
			}

		public:

			virtual void start() {}
			virtual void tick() {}

		protected:

			virtual void handle_message() {}
			virtual void handle_error(const asio::error_code& ec)
			{
				if (!m_tcp_socket.is_open())
				{
					return;
				}
				m_tcp_socket.close();
			}

		protected:

			int32_t	m_id = -1;
			ITCPConnectionParent*	m_tcp_conenction_parent;
			asio::io_context&		m_io_context;
			asio::ip::tcp::socket	m_tcp_socket;
		};
		using TCPConnectionPtr = std::shared_ptr<TCPConnection>;
	}
}