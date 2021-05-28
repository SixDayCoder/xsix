#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"

#include "asio.hpp"

namespace xsix
{
	namespace net
	{
		template<typename ConnectionT>
		class TCPServer : public xsix::INonCopyable
		{
		public:

			using ConnectionPtr = std::shared_ptr<ConnectionT>;

		public:

			TCPServer(asio::io_context& ctx, uint16_t port) :
				m_io_context(ctx),
				m_listen_ep(asio::ip::tcp::v4(), port),
				m_tcp_acceptor(ctx, m_listen_ep),
				m_quit(false)
			{
				asio::socket_base::reuse_address option(true);
				m_tcp_acceptor.set_option(option);
			}

		public:

			asio::io_context& get_context() { return m_io_context; }

		public:

			void start()
			{
				async_accept();
			}

			void loop()
			{
				m_quit.store(false);
				while (!m_quit.load())
				{
					if (!m_tcp_acceptor.is_open())
					{
						break;;
					}

					auto executed_events = m_io_context.poll();

					std::vector<ConnectionPtr> invalid_connection;
					invalid_connection.reserve(m_tcp_conn_map.size() / 2);

					//tick valid connection
					for (auto it = m_tcp_conn_map.begin(); it != m_tcp_conn_map.end(); it++)
					{
						if (it->second->is_valid())
						{
							it->second->tick();
						}
						else
						{
							invalid_connection.push_back(it->second);
						}	
					}

					//delete invalid connection
					for (auto connptr : invalid_connection)
					{
						handle_remove_connection(connptr);
					}
				}
			}

			void stop()
			{
				m_quit.store(false);
			}

		public:

			virtual void handle_accept_new_connection(ConnectionPtr connptr)
			{
				if (connptr)
				{
					add_conn(connptr);
					connptr->start();
				}
			}

			virtual void handle_remove_connection(ConnectionPtr connptr)
			{
				if (connptr)
				{
					remove_conn(connptr->get_id());
				}
			}

		private:

			ConnectionPtr create_new_connection()
			{
				ConnectionPtr connptr(new ConnectionT(m_io_context));
				return connptr;
			}

			void async_accept()
			{
				ConnectionPtr connptr = create_new_connection();
				m_tcp_acceptor.async_accept(connptr->socket(),
					[this, connptr](const asio::error_code& ec) {
						if (!connptr)
						{
							return;
						}
						if (!ec)
						{
							handle_accept_new_connection(connptr);
							async_accept();
						}
					}
				);
			}

		private:

			void add_conn(ConnectionPtr connptr)
			{
				if (connptr)
				{
					auto it = m_tcp_conn_map.find(m_next_conn_id);
					if (it != m_tcp_conn_map.end())
					{
						return;
					}
					connptr->set_id(m_next_conn_id++);
					m_tcp_conn_map.insert(std::make_pair(connptr->get_id(), connptr));
				}
			}

			void remove_conn(int32_t connid)
			{
				auto it = m_tcp_conn_map.find(connid);
				if (it != m_tcp_conn_map.end())
				{
					m_tcp_conn_map.erase(it);
				}
			}

			ConnectionPtr get_conn(int32_t connid)
			{
				auto it = m_tcp_conn_map.find(connid);
				if (it != m_tcp_conn_map.end())
				{
					return it->second;
				}
				return nullptr;
			}

		private:

			std::atomic<bool>		m_quit;
			asio::io_context&		m_io_context;
			asio::ip::tcp::endpoint m_listen_ep;
			asio::ip::tcp::acceptor m_tcp_acceptor;

		protected:
			int32_t							 m_next_conn_id = 0;
			std::map<int32_t, ConnectionPtr> m_tcp_conn_map;
		};
	}
}