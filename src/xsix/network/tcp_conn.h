#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/buffer.h"
#include "xsix/network/network.h"
#include "xsix/network/callback_define.h"
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
			Connecting = 0,
			Connected = 1,
			Disconnecting = 2,
			Disconnected = 3,
		};

	public:

		TCPConn(EventLoop* eventloop, int sockfd, int id);

		~TCPConn();
	
	public:

		const int	get_state() const { return m_state; }

		void		set_state(int state) { m_state = state; }

		const int	get_id() const { return m_id; }

		const int	get_fd() const;

	public:

		void set_recv_message_callback(TCPRecvMessageCallBack cb) { m_message_cb = cb; }

		void set_remove_conn_callback(TCPRemoveConnCallBack cb) { m_remove_conn_cb = cb; }

		void set_write_complete_callback(TCPWriteCompleteCallBack cb) { m_write_complete_cb; }

	public:
		
		void shutdown();

		void shutdown_in_loop();

	public:

		void send(xsix::buffer* buf);

		void send_in_loop(const char* data, int len);

	public:

		void set_nodelay() { m_tcp_socket->set_nodelay(true); }

		void set_nonblock() { m_tcp_socket->set_nonblock(true); }

		void set_keep_alive() { m_tcp_socket->set_keep_alive(); }

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

		int							m_id;

		EventLoop*					m_eventloop;

		std::unique_ptr<TCPSocket>	m_tcp_socket;

		std::unique_ptr<Channel>	m_channel;

		xsix::buffer				m_recv_buffer;

		xsix::buffer				m_send_buffer;

		TCPRecvMessageCallBack		m_message_cb;

		TCPRemoveConnCallBack       m_remove_conn_cb;

		TCPWriteCompleteCallBack	m_write_complete_cb;
	};
}