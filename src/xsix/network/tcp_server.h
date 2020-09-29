#pragma once

#include "xsix/network/network.h"
#include "xsix/buffer.h"

namespace xsix
{

	class TCPServer
	{
	public:

		TCPServer(uint16_t nBindPort, bool bUserIPv6 = false);

		virtual ~TCPServer();

	public:

		bool Listen(int32_t nBacklog = XSIX_DEFAULT_LISTEN_BACKLOG);

	public:

		void Run();

	private:

		void RunAccept();

	private:

		xsix::ServerSocket m_ServerSock;

		xsix::buffer m_SendBuffer;

		xsix::buffer m_RecvBuffer;

		xsix::NetAddr m_LocalAddr;

		bool m_bQuit;

	};

}