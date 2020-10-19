#include "xsix/network/tcp_conn.h"
#include "xsix/network/eventloop.h"
#include "xsix/network/channel.h"
#include <stdio.h>

namespace xsix
{

	TCPConn::TCPConn(EventLoop* eventloop, int sockfd, int id) :
		m_state(Connecting),
		m_eventloop(eventloop),
		m_tcp_socket(new TCPSocket(sockfd)),
		m_channel(new Channel(eventloop, sockfd)),
		m_id(id)
	{
		m_channel->set_read_cb(std::bind(&TCPConn::handle_read, this));
		m_channel->set_write_cb(std::bind(&TCPConn::handle_write, this));
		m_channel->set_error_cb(std::bind(&TCPConn::handle_error, this));
		m_channel->set_close_cb(std::bind(&TCPConn::handle_close, this));

		m_tcp_socket->set_nonblock(true);//非阻塞
		m_tcp_socket->set_nodelay(true);//nagle,小包合并
		m_tcp_socket->set_keep_alive();//heartbeat

		printf("[TCPConn] tcp conn ctor <id : %d, fd : %d>\n", m_id, m_tcp_socket->get_sockfd());
	}

	TCPConn::~TCPConn()
	{
		XASSERT(m_state == EState::Disconnected);
		printf("[TCPConn] tcp conn dtor <id : %d, fd : %d>\n", m_id, m_tcp_socket->get_sockfd());
	}

	const int TCPConn::get_fd() const
	{
		return m_channel->get_fd();
	}

	void TCPConn::send(xsix::buffer* buf)
	{
		m_send_buffer.clear();
		m_send_buffer.read_from(buf->retrieve_all(), buf->length());

		//TODO:
		m_recv_buffer.clear();

		//TODO:FIXME!!!!replace with send_in_loop
		if (m_eventloop->is_currthread_in_loopthread())
		{	
			handle_write();
		}
		else
		{
			m_eventloop->run_in_loop(std::bind(&TCPConn::handle_write, this));
		}	
	}

	void TCPConn::on_conn_established()
	{
		XASSERT(m_state == EState::Connecting);
		set_state(EState::Connected);
		m_channel->enable_read();
	}

	void TCPConn::on_conn_destoryed()
	{
		if (m_state == EState::Connected)
		{
			set_state(EState::Disconnected);

			m_channel->disable_read();
			m_channel->disable_write();
		}
		m_channel->remove_from_eventloop();
	}

	void TCPConn::handle_read()
	{
		char buf[65536] = { 0 };
		int32_t rc = xsix::socketapi::recvbytes(m_tcp_socket->get_sockfd(), buf, 65536);

		if (rc < 0)
		{
			handle_error();
			return;
		}

		if (rc == 0)
		{
			handle_close();
			return;
		}

		rc = m_recv_buffer.read_from(buf, rc);
		if (rc > 0)
		{
			if (m_message_cb)
			{
				m_message_cb(shared_from_this(), &m_recv_buffer, Timestamp::now());
			}
		}
	}

	void TCPConn::handle_write()
	{
		int32_t bytes = m_send_buffer.length();
		if (bytes <= 0)
		{
			return;
		}
		std::string s = m_send_buffer.retrieve_all_as_string();
		xsix::socketapi::sendbytes(m_tcp_socket->get_sockfd(), s.c_str(), s.length());
	}

	void TCPConn::handle_error()
	{
		if (m_remove_conn_cb)
		{
			printf("[TCPConn] handle error <id : %d, fd : %d>\n", m_id, m_tcp_socket->get_sockfd());
			m_remove_conn_cb(shared_from_this());
		}
	}

	void TCPConn::handle_close()
	{
		if (m_remove_conn_cb)
		{
			printf("[TCPConn] handle close <id : %d, fd : %d>\n", m_id, m_tcp_socket->get_sockfd());
			m_remove_conn_cb(shared_from_this());
		}
	}

}