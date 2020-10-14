#include "xsix/network/network.h"
#include "xsix/utils/string_utils.h"
#include <stdlib.h>
#include <string.h>

namespace xsix
{
	#pragma region NetAddr

	NetAddr::NetAddr(const std::string& ip, uint16_t port, bool ipv6 /*= false*/) : m_ipv6(ipv6)
	{
		memset(&m_ss, 0, sizeof(m_ss));
		if (ipv6)
		{
			struct sockaddr_in6* addr_v6 = (struct sockaddr_in6*) & m_ss;
			addr_v6->sin6_family = AF_INET6;
			addr_v6->sin6_port = htons(port);
			XASSERT(inet_pton(AF_INET6, ip.c_str(), &(addr_v6->sin6_addr)));
		}
		else
		{
			struct sockaddr_in* addr_v4 = (struct sockaddr_in*) & m_ss;
			addr_v4->sin_family = AF_INET;
			addr_v4->sin_port = htons(port);
			XASSERT(inet_pton(AF_INET, ip.c_str(), &(addr_v4->sin_addr)));
		}
	}

	std::string NetAddr::ipaddress() const
	{
		if (m_ss.ss_family == AF_INET && !m_ipv6)
		{
			struct sockaddr_in* addr4 = (struct sockaddr_in*) & m_ss;
			char buf[INET_ADDRSTRLEN] = {};
			const char* addr = inet_ntop(m_ss.ss_family, &addr4->sin_addr, buf, INET_ADDRSTRLEN);
			return addr ? std::string(addr) : "";
		}
		else if (m_ss.ss_family == AF_INET6 && m_ipv6)
		{
			struct sockaddr_in6* addr6 = (struct sockaddr_in6*) & m_ss;
			char buf[INET6_ADDRSTRLEN] = {};
			const char* addr = inet_ntop(m_ss.ss_family, &addr6->sin6_addr, buf, INET6_ADDRSTRLEN);
			return addr ? std::string(addr) : "";
		}
		return "";
	}

	uint16_t NetAddr::port() const
	{
		if (m_ss.ss_family == AF_INET && !m_ipv6)
		{
			struct sockaddr_in* addr4 = (struct sockaddr_in*) & m_ss;
			return ntohs(addr4->sin_port);
		}
		else if (m_ss.ss_family == AF_INET6 && m_ipv6)
		{
			struct sockaddr_in6* addr6 = (struct sockaddr_in6*) & m_ss;
			return ntohs(addr6->sin6_port);
		}
		return 0;
	}

	#pragma  endregion

	bool TCPClientSocket::connect_ex(const NetAddr& addr)
	{
		int32_t addrsize = addr.get_ipv6() ? sizeof(sockaddr_in6) : sizeof(sockaddr_in);
		int32_t rc = connect(m_fd, (const sockaddr*)(&addr.get_sockaddr_storage()), addrsize);
		if (rc != 0)
		{
			socketapi::log_socket_error();
			return false;
		}
		return true;
	}

	SOCKET TCPServerSocket::accept_ex(NetAddr* addr)
	{
		XASSERT(addr);
		struct sockaddr_storage ss;
		socklen_t addrsize = m_ipv6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in);

		SOCKET fd = accept(m_fd, (sockaddr*)(&ss), &addrsize);
		if (fd == INVALID_SOCKET)
		{
			socketapi::log_socket_error();
			return INVALID_SOCKET;
		}
		addr->set_sockaddr_storage(ss);
		addr->set_ipv6(m_ipv6);
		return fd;
	}
}