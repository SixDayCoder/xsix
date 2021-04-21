#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include "xsix/time/timestamp.h"
#include "xsix/buffer.h"

#include "3rd/asio/include/asio.hpp"

#include <memory>

namespace xsix
{
	class TCPConn : public INonCopyable, public std::enable_shared_from_this<TCPConn>
	{
	public:

		TCPConn(asio::io_context& ctx) : m_tcp_socket(ctx) {}

	private:

		asio::ip::tcp::socket m_tcp_socket;

	};
}