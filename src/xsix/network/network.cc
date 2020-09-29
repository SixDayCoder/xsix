#include "xsix/network/network.h"
#include "xsix/utils/string_utils.h"
#include <stdlib.h>
#include <string.h>

namespace xsix
{

	Socket::Socket(bool bUseIPv6 /*= false*/) :
		m_fd(INVALID_SOCKET),
		m_bUseIPv6(bUseIPv6)
	{
		m_fd = socketapi::create_socket(bUseIPv6);
		XASSERT(m_fd != INVALID_SOCKET);
	}

	bool Socket::SetNonBlock(bool on)
	{
		return socketapi::set_nonblock(m_fd, on);
	}

	bool Socket::SetTCPNoDelay(bool on)
	{
		return socketapi::set_tcp_nodelay(m_fd, on);
	}

	void Socket::Close()
	{
		if (m_fd != INVALID_SOCKET)
		{
			socketapi::close_socket(m_fd);
		}
	}

	int32_t Socket::RecvBytes(char* buffer, int32_t nCount)
	{
		return socketapi::recv_bytes(m_fd, buffer, nCount);
	}

	int32_t Socket::SendBytes(const char* buffer, int32_t nCount)
	{
		return socketapi::send_bytes(m_fd, buffer, nCount);
	}

	bool ClientSocket::Connect(const NetAddr& addr)
	{
		int32_t nSockAddrSize = addr.ipv6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in);
		int32_t rc = connect(m_fd, (const sockaddr*)(&addr.ss), nSockAddrSize);
		if (rc != 0)
		{
			socketapi::log_socket_error();
			return false;
		}
		return true;
	}

	bool ServerSocket::SetReusePort()
	{
		return socketapi::set_reuse_port(m_fd);
	}

	bool ServerSocket::SetReuseAddr()
	{
		return socketapi::set_reuse_addr(m_fd);
	}

	bool ServerSocket::Bind(uint16_t port)
	{
		m_Port = port;
		return socketapi::bind_ex(m_fd, m_Port, m_bUseIPv6);
	}

	bool ServerSocket::Listen(int32_t backlog /*= 128*/)
	{
		return socketapi::listen_ex(m_fd, backlog);
	}

	SOCKET ServerSocket::Accept(NetAddr* addr)
	{
		XASSERT(addr);
		sockaddr_storage ss;
		memset(&ss, 0, sizeof(ss));
		socklen_t len = sizeof(ss);
		SOCKET fd = accept(m_fd, (sockaddr*)&ss, &len);
		if (fd == INVALID_SOCKET)
		{
			socketapi::log_socket_error();
			return INVALID_SOCKET;
		}
		addr->ipv6 = m_bUseIPv6;
		addr->ss = ss;
		return fd;
	}

	//******************************************************net addr******************************************************//

	NetAddr::NetAddr(const std::string& ip, uint16_t port, bool bUseIPv6 /*= false*/) : ipv6(bUseIPv6)
	{
		memset(&ss, 0, sizeof(ss));
		if (bUseIPv6)
		{
			struct sockaddr_in6* addr_v6 = (struct sockaddr_in6*) & ss;
			addr_v6->sin6_family = AF_INET6;
			addr_v6->sin6_port = htons(port);
			XASSERT(inet_pton(AF_INET6, ip.c_str(), &(addr_v6->sin6_addr)));
		}
		else
		{
			struct sockaddr_in* addr_v4 = (struct sockaddr_in*) & ss;
			addr_v4->sin_family = AF_INET;
			addr_v4->sin_port = htons(port);
			XASSERT(inet_pton(AF_INET, ip.c_str(), &(addr_v4->sin_addr)));
		}
	}

	NetAddr::NetAddr(bool bUseIPv6 /*= false*/) : ipv6(bUseIPv6)
	{
		memset(&ss, 0, sizeof(ss));
	}

	std::string NetAddr::IP()
	{
		if (ss.ss_family == AF_INET && !ipv6)
		{
			struct sockaddr_in* addr4 = (struct sockaddr_in*) & ss;
			char buf[INET_ADDRSTRLEN] = {};
			const char* addr = inet_ntop(ss.ss_family, &addr4->sin_addr, buf, INET_ADDRSTRLEN);
			return addr ? std::string(addr) : "";
		}
		else if (ss.ss_family == AF_INET6 && ipv6)
		{
			struct sockaddr_in6* addr6 = (struct sockaddr_in6*) & ss;
			char buf[INET6_ADDRSTRLEN] = {};
			const char* addr = inet_ntop(ss.ss_family, &addr6->sin6_addr, buf, INET6_ADDRSTRLEN);
			return addr ? std::string(addr) : "";
		}
		return "";
	}

	uint16_t NetAddr::Port()
	{
		if (ss.ss_family == AF_INET && !ipv6)
		{
			struct sockaddr_in* addr4 = (struct sockaddr_in*) & ss;
			return ntohs(addr4->sin_port);
		}
		else if (ss.ss_family == AF_INET6 && ipv6)
		{
			struct sockaddr_in6* addr6 = (struct sockaddr_in6*) & ss;
			return ntohs(addr6->sin6_port);
		}
		return 0;
	}

	bool NetAddr::IsIPv4Addr(const std::string& ip)
	{
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		int32_t rc = inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
		return rc == 0 ? true : false;
	}

	bool NetAddr::IsIPv6Addr(const std::string& ip)
	{
		struct sockaddr_in6 addr;
		memset(&addr, 0, sizeof(addr));
		int32_t rc = inet_pton(AF_INET6, ip.c_str(), &addr.sin6_addr);
		return rc == 0 ? true : false;
	}

}