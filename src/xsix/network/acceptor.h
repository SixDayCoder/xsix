#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/network/network.h"
#include "xsix/network/channel.h"

namespace xsix
{
	class EventLoop;

	class Acceptor : public INonCopyable
	{
	public:

		using NewConnCallBack = std::function<void(int, const NetAddr& addr)>;

	public:

		Acceptor(EventLoop* eventloop, uint16_t listen_port);

		~Acceptor();

	public:

		void set_new_conn_callback(NewConnCallBack cb) { m_new_conn_cb = cb; }

		void listen();

	private:

		void handle_new_conn();

	private:

		EventLoop*		m_eventloop;

		TCPServerSocket	m_listen_socket;

		Channel			m_accept_channel;

		NewConnCallBack	m_new_conn_cb;
	};
}