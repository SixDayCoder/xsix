#pragma once

#include "xsix/common_define.h"
#include "xsix/time/timestamp.h"

namespace xsix
{
	class buffer;

	class TCPConn;
	using TCPConnPtr = std::shared_ptr<TCPConn>;
	using TCPRecvMessageCallBack = std::function<void(const xsix::TCPConnPtr&, xsix::buffer*, xsix::Timestamp) >;
	using TCPRemoveConnCallBack = std::function<void(const xsix::TCPConnPtr&)>;

	class Channel;
	using ChannelEventCallBack = std::function<void()>;


}