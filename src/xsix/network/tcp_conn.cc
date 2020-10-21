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
		if (m_state == TCPConn::EState::Connected)
		{
			if (m_eventloop->is_currthread_in_loopthread())
			{
				//sync
				send_in_loop(buf->retrieve_all_as_string().c_str(), buf->length());
				buf->clear();
			}
			else
			{
				//async
				std::string msg(buf->retrieve_all_as_string());
				buf->clear();
				m_eventloop->run_in_loop(std::bind(&TCPConn::send_in_loop, this, msg.c_str(), msg.length()));
			}
		}
	}

	void TCPConn::send_in_loop(const char* data, int len)
	{
		/******************************************************************
		//1.如果channel没有监听写事件,尽可能的向fd写data
		//2.向fd写data如果有剩余,将剩余数据加到m_send_buffer中,开启监听写事件
		//3.写完毕后触发write_complete_cb,并且移除写事件
		*******************************************************************/
	
		int nwrite = 0;
		int remain = 0;

		//没有监听写事件且写缓冲没有数据
		if (!m_channel->is_writing() && m_send_buffer.length() == 0)
		{
			nwrite = socketapi::sendbytes(m_channel->get_fd(), data, len);

			//write success
			if (nwrite >= 0)
			{
				remain = len - nwrite;
				if (remain == 0 && m_write_complete_cb)
				{
					m_eventloop->run_in_loop(std::bind(m_write_complete_cb, shared_from_this()));
				}
			}
			//write error
			else
			{
				//TODO:handle_error
			}
		}

		//剩余数据添加到m_send_buffer
		if (remain > 0)
		{
			m_send_buffer.append(data + nwrite, remain);
			if (!m_channel->is_writing())
			{
				m_channel->enable_write();
			}
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

		rc = m_recv_buffer.append(buf, rc);
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
		if (m_channel->is_writing())
		{
			std::string msg(m_send_buffer.retrieve_all_as_string());
			int32_t n = socketapi::sendbytes(m_channel->get_fd(), msg.c_str(), msg.length());

			//write success
			if (n > 0)
			{
				//m_send_buffer中的数据全部发送完
				if (n == msg.length())
				{
					m_channel->disable_write();
					m_send_buffer.clear();

					if (m_write_complete_cb)
					{
						m_eventloop->run_in_loop(std::bind(m_write_complete_cb, shared_from_this()));
					}
				}
				//如果没有全部发完,m_channel还是监听了writing,会在下一次poll之后handle_write
				//需要维护m_send_buffer的状态
				else
				{
					m_send_buffer.clear();
					m_send_buffer.append(msg.c_str() + n, msg.length() - n);
				}
			}
			//write error
			else
			{
				//TODO:handle error
			}
		}
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