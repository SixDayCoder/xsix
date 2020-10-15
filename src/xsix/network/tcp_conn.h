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

	class TCPConn;
	using TCPConnPtr = std::shared_ptr<TCPConn>;

	class TCPConn : public INonCopyable, public std::enable_shared_from_this<TCPConn>
	{
	public:

		enum EState
		{
			Connecting = 0,
			Connected = 1,
			Disconnecting = 2,
			Disconnected = 3,
		};

	public:

		using MessageCallBack = std::function<void (const xsix::TCPConnPtr&, xsix::buffer*, xsix::Timestamp) >;

	public:

		TCPConn(EventLoop* eventloop, int sockfd);

		~TCPConn();
	
	public:

		int	 get_state() const { return m_state; }

		void set_state(int state) { m_state = state; }

	public:

		void set_message_callback(MessageCallBack cb) { m_message_cb = cb; }

	public:

		void send(xsix::buffer* buf);

	public:

		void on_conn_established();

		void on_conn_destoryed();

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

		MessageCallBack				m_message_cb;
	};
}