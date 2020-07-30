#include "xsix/xsix_network.h"
#include "xsix/xsix_string_utils.h"
#include <stdlib.h>
#include <string.h>


namespace xsix
{

	#ifndef _XSIX_SOCK_ERROR_MSG
	#define _XSIX_SOCK_ERROR_MSG(name, msg) \
			case name:\
				msg = #name;\
				break;
	#endif


	void NetworkEnv::Init()
	{
#if defined(_XSIX_WINDOWS)
		WSADATA wsaData;
		XASSERT(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#endif
	}

	void NetworkEnv::Clean()
	{
#if defined(_XSIX_WINDOWS)
		WSACleanup();
#endif
	}

	//******************************************************socket api******************************************************//

	void SocketAPI::LogSocketError()
	{
		std::string szErrorMsg = "UNKNOWN";
		int32_t nErrorCode = -1;

#if   defined (_XSIX_WINDOWS)
		nErrorCode = WSAGetLastError();
		switch (nErrorCode)
		{
			_XSIX_SOCK_ERROR_MSG(WSANOTINITIALISED, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAENETDOWN, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEAFNOSUPPORT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEINPROGRESS, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEMFILE, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAENOBUFS, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEPROTONOSUPPORT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEPROTOTYPE, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAESOCKTNOSUPPORT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEADDRINUSE, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEADDRNOTAVAIL, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEFAULT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEINVAL, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAENOTSOCK, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEINTR, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEALREADY, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAECONNREFUSED, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAENETUNREACH, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAETIMEDOUT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEWOULDBLOCK, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEOPNOTSUPP, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAENOPROTOOPT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEACCES, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAENETRESET, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAENOTCONN, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAESHUTDOWN, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEMSGSIZE, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAEHOSTUNREACH, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAECONNABORTED, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(WSAECONNRESET, szErrorMsg);
		default:
			break;
		}

#elif defined(_XSIX_LINUX)
		nErrorCode = errno;
		switch (nErrorCode)
		{
			_XSIX_SOCK_ERROR_MSG(EALREADY, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(ECONNABORTED, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(ECONNREFUSED, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(ECONNRESET, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(ETIMEDOUT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(EFAULT, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(ENOTSOCK, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(EINPROGRESS, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(EWOULDBLOCK, szErrorMsg);
			_XSIX_SOCK_ERROR_MSG(EREMOTE, szErrorMsg);
		default:
			break;
		}
#endif
		printf("error_code : %d, error_msg : %s\n", nErrorCode, szErrorMsg.c_str());

	}

	void SocketAPI::CloseSocket(SOCKET fd)
	{
#if defined(_XSIX_WINDOWS)
		closesocket(fd);
#else 
		close(fd);
#endif
	}

	static bool SetSocketOpt(SOCKET fd, int32_t socktype, int32_t opname, void* opval, uint32_t oplen)
	{
#if defined(_XSIX_WINDOWS)
		int32_t rc = setsockopt(fd, socktype, opname, (char*)opval, oplen);
		if (rc == SOCKET_ERROR)
		{
			SocketAPI::LogSocketError();
			return false;
		}
		return true;
#elif defined(_XSIX_LINUX)
		int32_t rc = setsockopt(fd, socktype, opname, opval, oplen);
		if (rc == SOCKET_ERROR)
		{
			return false;
		}
		return true;
#endif
		return false;
	}

	SOCKET SocketAPI::CreateSocket(bool bUseIPv6)
	{
		int32_t nDomain = bUseIPv6 ? AF_INET6 : AF_INET;
		SOCKET sock = socket(nDomain, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			SocketAPI::LogSocketError();
		}
		return sock;
	}

	bool SocketAPI::SetNonBlock(SOCKET fd, bool on)
	{
		if (fd == INVALID_SOCKET)
		{
			return false;
		}

#if defined(_XSIX_WINDOWS)

		unsigned long ul = on ? 1 : 0;
		int32_t rc = ioctlsocket(fd, FIONBIO, (unsigned long *)&ul);
		if (rc < 0)
		{
			SocketAPI::LogSocketError();
		}
		return rc < 0 ? false : true;

#elif defined(_XSIX_LINUX)

		int32_t flags = fcntl(fd, F_GETFL, 0);
		if (on)
		{
			flags |= O_NONBLOCK;
		}
		else
		{
			flags &= ~O_NONBLOCK;
		}
		int32_t rc = fcntl(fd, F_SETFL, flags);
		if (rc < 0)
		{
			SocketAPI::LogSocketError();
		}
		return rc < 0 ? false : true;
#endif

		return false;
	}

	bool SocketAPI::SetReuseAddr(SOCKET fd)
	{
		int32_t opval = 1;
		return SetSocketOpt(fd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(opval));
	}

	bool SocketAPI::SetReusePort(SOCKET fd)
	{
#ifdef SO_REUSEPORT
		int32_t opval = 1;
		return SetSocketOpt(fd, SOL_SOCKET, SO_REUSEPORT, &opval, sizeof(opval));
#endif
		return false;
	}

	bool SocketAPI::SetLinger(SOCKET fd, uint32_t nLingerTime)
	{
		struct linger socklinger;
		socklinger.l_onoff = nLingerTime > 0 ? 1 : 0;
		socklinger.l_linger = nLingerTime > 0 ? nLingerTime : 0;
		return SetSocketOpt(fd, SOL_SOCKET, SO_LINGER, &socklinger, sizeof(socklinger));
	}

	bool SocketAPI::SetTCPNoDelay(SOCKET fd, bool on)
	{
		int32_t opval = on ? 1 : 0;
		return SetSocketOpt(fd, IPPROTO_TCP, TCP_NODELAY, &opval, sizeof(opval));
	}

	bool SocketAPI::Bind(SOCKET fd, uint16_t port, bool bUseIPv6 /*= false*/)
	{
		if (bUseIPv6)
		{
			sockaddr_in6 addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
			addr.sin6_addr = in6addr_any;
			int32_t rc = bind(fd, (const sockaddr*)&addr, sizeof(addr));
			if (rc == SOCKET_ERROR)
			{
				LogSocketError();
				return false;
			}
			return true;
		}
		else
		{
			sockaddr_in addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			int32_t rc = bind(fd, (const sockaddr*)&addr, sizeof(addr));
			if (rc == SOCKET_ERROR)
			{
				LogSocketError();
				return false;
			}
			return true;
		}
	}

	bool SocketAPI::Listen(SOCKET fd, int32_t nBacklog)
	{
		int32_t rc = listen(fd, nBacklog);
		if (rc == SOCKET_ERROR)
		{
			LogSocketError();
			return false;
		}
		return true;
	}

	int32_t SocketAPI::RecvBytes(SOCKET fd, char* buffer, int32_t nCount)
	{
		if (fd == INVALID_SOCKET)
		{
			return SOCKET_ERROR;
		}
		if (!buffer || nCount <= 0)
		{
			SocketAPI::LogSocketError();
			return SOCKET_ERROR;
		}
		auto rc = recv(fd, buffer, nCount, 0);
		if (rc == SOCKET_ERROR)
		{
			SocketAPI::LogSocketError();
			return SOCKET_ERROR;
		}
		return rc;
	}

	int32_t SocketAPI::SendBytes(SOCKET fd, const char* buffer, int32_t nCount)
	{
		if (fd == INVALID_SOCKET)
		{
			return SOCKET_ERROR;
		}
		auto rc = send(fd, buffer, nCount, 0);
		if (rc == SOCKET_ERROR)
		{
			SocketAPI::LogSocketError();
			return SOCKET_ERROR;
		}
		return rc;
	}

	//******************************************************socket******************************************************//

	Socket::Socket(bool bUseIPv6 /*= false*/):
			m_fd(INVALID_SOCKET),
		    m_bUseIPv6(bUseIPv6)
	{
		m_fd = SocketAPI::CreateSocket(m_bUseIPv6);
		XASSERT(m_fd != INVALID_SOCKET);
	}

	bool Socket::SetNonBlock(bool on)
	{
		return SocketAPI::SetNonBlock(m_fd, on);
	}

	bool Socket::SetTCPNoDelay(bool on)
	{
		return SocketAPI::SetTCPNoDelay(m_fd, on);
	}

	void Socket::Close()
	{
		if (m_fd != INVALID_SOCKET)
		{
			SocketAPI::CloseSocket(m_fd);
		}
	}

	int32_t Socket::RecvBytes(char* buffer, int32_t nCount)
	{
		return SocketAPI::RecvBytes(m_fd, buffer, nCount);
	}

	int32_t Socket::SendBytes(const char* buffer, int32_t nCount)
	{
		return SocketAPI::SendBytes(m_fd, buffer, nCount);
	}

	bool ClientSocket::Connect(const NetAddr& addr)
	{
		int32_t nSockAddrSize = addr.ipv6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in);
		int32_t rc = connect(m_fd, (const sockaddr*)(&addr.ss), nSockAddrSize);
		if (rc != 0)
		{
			SocketAPI::LogSocketError();
			return false;
		}
		return true;
	}

	bool ServerSocket::SetReusePort()
	{
		return SocketAPI::SetReuseAddr(m_fd);
	}

	bool ServerSocket::SetReuseAddr()
	{
		return SocketAPI::SetReuseAddr(m_fd);
	}

	bool ServerSocket::Bind(uint16_t port)
	{
		m_Port = port;
		return SocketAPI::Bind(m_fd, m_Port, m_bUseIPv6);
	}

	bool ServerSocket::Listen(int32_t backlog /*= 128*/)
	{
		return SocketAPI::Listen(m_fd, backlog);
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
			SocketAPI::LogSocketError();
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
			struct sockaddr_in6 *addr_v6 = (struct sockaddr_in6 *)&ss;
			addr_v6->sin6_family = AF_INET6;
			addr_v6->sin6_port = htons(port);
			XASSERT(inet_pton(AF_INET6, ip.c_str(), &(addr_v6->sin6_addr)));
		}
		else
		{
			struct sockaddr_in *addr_v4 = (struct sockaddr_in *)&ss;
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
			struct sockaddr_in* addr4 = (struct sockaddr_in*)&ss;
			char buf[INET_ADDRSTRLEN] = {};
			const char* addr = inet_ntop(ss.ss_family, &addr4->sin_addr, buf, INET_ADDRSTRLEN);
			return addr ? std::string(addr) : "";
		}
		else if (ss.ss_family == AF_INET6 && ipv6)
		{
			struct sockaddr_in6* addr6 = (struct sockaddr_in6*)&ss;
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
			struct sockaddr_in* addr4 = (struct sockaddr_in*)&ss;
			return ntohs(addr4->sin_port);
		}
		else if (ss.ss_family == AF_INET6 && ipv6)
		{
			struct sockaddr_in6* addr6 = (struct sockaddr_in6*)&ss;
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