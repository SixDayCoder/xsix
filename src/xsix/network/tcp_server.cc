#include "tcp_server.h"
#include <iostream>

namespace xsix
{

	TCPServer::TCPServer(asio::io_context& ctx, uint16_t port) :
				m_ctx(ctx),
				m_listen_ep(asio::ip::tcp::v4(), port),
				m_tcp_acceptor(ctx, m_listen_ep) {}

	void TCPServer::set_reuse_addr(bool reuse)
	{
		asio::socket_base::reuse_address option(reuse);
		m_tcp_acceptor.set_option(option);
	}

	void TCPServer::accept()
	{
		auto self(shared_from_this());

		TCPConnPtr conn(new TCPConn(m_ctx));	
		conn->set_state(TCPConn::STATE_CONNECTING);
		conn->set_message_handler(m_conn_message_handler);
		conn->set_close_handler([self](TCPConnPtr ptr, const asio::error_code& ec) {
			if (!self || !ptr)
			{
				return;
			}
			self->m_tcp_conn_mgr.remove_conn(ptr->get_id());
			if (self->m_conn_close_handler)
			{
				self->m_conn_close_handler(ptr, ec);
			}
		});

		//fire another accept events
		m_tcp_acceptor.async_accept(conn->socket(),
			[self, conn](const asio::error_code& ec) {
				if (!self || !conn)
				{
					return;
				}
				if (!ec)
				{
					self->m_tcp_conn_mgr.add_conn(conn);
					if (self->m_accept_handler)
					{
						self->m_accept_handler(self, conn, ec);
					}
					conn->set_state(TCPConn::STATE_CONNECTED);
					conn->start();

					//fire another accept event
					self->accept();
				}
			}
		);
	}

	void TCPServer::loop()
	{
		while (m_tcp_acceptor.is_open())
		{
			tick();
		}
	}

	void TCPServer::tick()
	{
		if (!m_tcp_acceptor.is_open())
		{
			return;
		}

		//poll for async_accept/conntions async_recv/async_send
		auto executed_events = m_ctx.poll();
		//connection tick 
		for (auto it = m_tcp_conn_mgr.m_tcp_conn_map.begin(); 
				  it != m_tcp_conn_mgr.m_tcp_conn_map.end(); it++)
		{
			it->second->tick();
		}
	}

	void TCPServer::broadcast(const char* msg, int32_t size)
	{
		for (auto it = m_tcp_conn_mgr.m_tcp_conn_map.begin();
			it != m_tcp_conn_mgr.m_tcp_conn_map.end(); it++)
		{
			it->second->send(msg, size);
		}
	}

}