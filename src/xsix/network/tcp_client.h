#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/buffer.h"

#include <atomic>
#include <memory>
#include <map>
#include <functional>
#include <stdio.h>

#include "asio.hpp"

namespace xsix
{
	class TCPClient : public xsix::INonCopyable
	{
	public:

		using MsgProcessor = std::function<void(xsix::buffer& msgbuffer)>;

	public:

		TCPClient(asio::io_context& ctx);

	public:

		void set_msg_handler(MsgProcessor msgprocessor) { m_msg_processor = msgprocessor; }

	public:

		void connect(asio::ip::tcp::endpoint ep);

		void send(const char* msg, std::size_t msgsize);

		void recv();

		void loop();

		void tick();

	private:

		void default_msg_process(xsix::buffer& msgbuffer);

		void send_buffered_msg();

	private:

		void handle_error(const asio::error_code& ec);

	private:

		asio::io_context&	  m_ctx;
		asio::ip::tcp::socket m_tcp_socket;
	
		xsix::buffer		  m_send_buffer;
		xsix::buffer		  m_recv_buffer;

		MsgProcessor		  m_msg_processor;
	};
}
