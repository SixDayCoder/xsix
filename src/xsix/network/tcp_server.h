#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/network/tcp_conn.h"
#include <atomic>
#include <memory>
#include <map>

namespace xsix
{

	class EventLoop;

	class Acceptor;

	class TCPServer
	{

	public:

		TCPServer(EventLoop* eventloop, uint16_t listen_port);

		~TCPServer();

	public:

		void run();

	public:

		void set_conn_message_cb(TCPConn::MessageCallBack cb) { m_conn_message_cb = cb; }

	private:

		void new_conn(int sockfd, const NetAddr& addr);

	private:

		EventLoop*						m_eventloop;

		std::unique_ptr<Acceptor>		m_acceptor;

		std::atomic<int32_t>			m_next_conn_id;

		std::map<int32_t, TCPConnPtr>	m_conn_map;

		TCPConn::MessageCallBack		m_conn_message_cb;
	};

}

