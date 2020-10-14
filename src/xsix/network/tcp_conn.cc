#include "xsix/network/tcp_conn.h"
#include "xsix/network/eventloop.h"
#include "xsix/network/channel.h"
#include <stdio.h>

namespace xsix
{

	TCPConn::TCPConn(EventLoop* eventloop, int sockfd) :
				m_state(CONNECTING),
				m_eventloop(eventloop),
				m_tcp_socket(new TCPSocket(sockfd)),
				m_channel(new Channel(eventloop, sockfd))
	{
		m_channel->set_read_cb(std::bind(&TCPConn::handle_read, this));
		m_channel->set_write_cb(std::bind(&TCPConn::handle_write, this));
		m_channel->set_error_cb(std::bind(&TCPConn::handle_error, this));
		m_channel->set_close_cb(std::bind(&TCPConn::handle_close, this));

		m_tcp_socket->set_nonblock(true);

		printf("tcp conn ctor with fd : %d\n", m_tcp_socket->get_sockfd());
	}

	TCPConn::~TCPConn()
	{
		printf("tcp conn dtor with fd : %d\n", m_tcp_socket->get_sockfd());
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

		m_recv_buffer.read_from(buf, rc);
	}

	void TCPConn::handle_write()
	{
		int32_t bytes = m_send_buffer.length();
		if (bytes <= 0)
		{
			return;
		}

		m_send_buffer.neaten();
		xsix::socketapi::sendbytes(m_tcp_socket->get_sockfd(), m_send_buffer.buf(), bytes);
	}

	void TCPConn::handle_error()
	{
		printf("tcp conn handle error with fd : %d\n", m_tcp_socket->get_sockfd());
	}

	void TCPConn::handle_close()
	{
		printf("tcp conn handle close with fd : %d\n", m_tcp_socket->get_sockfd());
	}

}