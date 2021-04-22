#include "tcp_server.h"
#include <stdio.h>

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

	void TCPServer::default_accept_handler(TCPConnPtr connptr, const asio::error_code& ec)
	{
		m_tcp_conn_mgr.add_conn(connptr);

		printf("new conn (id:%d, ip:%s, port:%d)\n", 
			connptr->get_id(), 
			connptr->ip().c_str(), 
			connptr->port()
		);

		TCPConnPtr newconn(new TCPConn(m_ctx));
		newconn->set_on_close(std::bind(&TCPServer::on_conn_close, this, std::placeholders::_1));
		m_tcp_acceptor.async_accept(newconn->socket(),
			std::bind(&TCPServer::default_accept_handler, this, newconn, std::placeholders::_1)
		);
	}

	void TCPServer::on_conn_close(TCPConnPtr ptr)
	{
		if (ptr)
		{
			m_tcp_conn_mgr.remove_conn(ptr->get_id());
		}
	}

	void TCPServer::accept()
	{
		TCPConnPtr ptr(new TCPConn(m_ctx));
		ptr->set_on_close(std::bind(&TCPServer::on_conn_close, this, std::placeholders::_1));

		if (m_accept_handler)
		{
			m_tcp_acceptor.async_accept(ptr->socket(),
				std::bind(m_accept_handler, ptr, std::placeholders::_1)
			);
		}
		else
		{
			m_tcp_acceptor.async_accept(ptr->socket(),
				std::bind(&TCPServer::default_accept_handler, this, ptr, std::placeholders::_1)
			);
		}
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
		//run for accept new connection
		std::size_t executed_events_count = m_ctx.run_for(std::chrono::milliseconds(20));
		printf("events : %d  conns : %d\n", executed_events_count, m_tcp_conn_mgr.m_tcp_conn_map.size());

		//tick 
		for (auto it = m_tcp_conn_mgr.m_tcp_conn_map.begin(); it != m_tcp_conn_mgr.m_tcp_conn_map.end(); it++)
		{
			it->second->tick();
		}
	}

}