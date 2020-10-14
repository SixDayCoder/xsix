#include "xsix/network/acceptor.h"
#include "xsix/network/eventloop.h"

namespace xsix
{

	Acceptor::Acceptor(EventLoop* eventloop, uint16_t listen_port) :
				m_eventloop(eventloop),
				m_listen_socket(),
				m_accept_channel(eventloop, m_listen_socket.get_sockfd()),
				m_new_conn_cb(nullptr)
	{
		m_listen_socket.set_nonblock(true);
		m_listen_socket.bind_ex(listen_port);
		m_listen_socket.set_reuse_port();
		m_accept_channel.set_read_cb(std::bind(&Acceptor::handle_new_conn, this));
	}

	Acceptor::~Acceptor()
	{

	}

	void Acceptor::listen()
	{
		m_accept_channel.update_to_eventloop();
		m_listen_socket.listen_ex();
	}

	void Acceptor::handle_new_conn()
	{
		NetAddr addr;
		int sockfd = m_listen_socket.accept_ex(&addr);
		if (sockfd >= 0)
		{
			if (m_new_conn_cb)
			{
				m_new_conn_cb(sockfd, addr);
			}
			else
			{
				xsix::socketapi::close_socket(sockfd);
			}
		}
	}

}