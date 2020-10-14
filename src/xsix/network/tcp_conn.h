#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/network/network.h"
#include "xsix/buffer.h"
#include <memory>

namespace xsix
{
	class EventLoop;

	class Channel;

	class TCPSocket;

	class TCPConn : public INonCopyable, public std::enable_shared_from_this<TCPConn>
	{
	public:

		enum EState
		{
			CONNECTING = 0,
		};

	public:

		TCPConn(EventLoop* eventloop, int sockfd);

		~TCPConn();

	
	public:

		int get_state() const { return m_state; }

	private:

		void handle_read();

		void handle_write();

		void handle_error();

		void handle_close();

	private:

		int							m_state;

		EventLoop*					m_eventloop;

		std::unique_ptr<TCPSocket>	m_tcp_socket;

		std::unique_ptr<Channel>	m_channel;

		xsix::buffer				m_recv_buffer;

		xsix::buffer				m_send_buffer;
	};
	using TCPConnPtr = std::shared_ptr<TCPConn>;
}