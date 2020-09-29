#include "xsix/network/tcp_server.h"
#include "xsix/time/timestamp.h"

namespace xsix
{
	TCPServer::TCPServer(uint16_t nBindPort, bool bUserIPv6 /*= false*/) : 
			   m_ServerSock(bUserIPv6),
			   m_SendBuffer(),
			   m_RecvBuffer(),
			   m_LocalAddr("0.0.0.0", nBindPort, bUserIPv6),
			   m_bQuit(false)
	{
		m_ServerSock.SetReuseAddr();
		XASSERT(m_ServerSock.Bind(nBindPort));
	}

	TCPServer::~TCPServer()
	{
		m_ServerSock.Close();
	}

	bool TCPServer::Listen(int32_t nBacklog /*= XSIX_DEFAULT_LISTEN_BACKLOG*/)
	{
		return m_ServerSock.Listen(nBacklog);
	}

	void TCPServer::Run()
	{
		while (!m_bQuit)
		{
			Timestamp::PrintNow();
			RunAccept();
		}
	}

	void TCPServer::RunAccept()
	{
		xsix::NetAddr addr;
		SOCKET fd = m_ServerSock.Accept(&addr);
		if (fd != INVALID_SOCKET)
		{
			printf("coming new conn (%s:%d)\n", addr.IP().c_str(), addr.Port());
			xsix::Socket sock(fd);
			char buffer[1024] = { 0 };
			int32_t nRecv = sock.RecvBytes(buffer, 1024);
			if (nRecv != SOCKET_ERROR)
			{
				if (nRecv == 0)
				{
					printf("client socket close\n");
					sock.Close();
				}
				else
				{
					printf("recv from (%s:%d) %d bytes, content is %s\n", addr.IP().c_str(), addr.Port(), nRecv, buffer);
					std::string reply = "echo " + std::string(buffer);
					sock.SendBytes(reply.c_str(), reply.size());
					sock.Close();
				}
			}
		}
	}
}