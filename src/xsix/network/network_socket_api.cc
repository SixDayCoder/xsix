#include "xsix/network/network.h"
#include "xsix/utils/string_utils.h"
#include <stdlib.h>
#include <string.h>

#define __XSIX_SOCK_ERROR_MSG(_error_code_, _output_message_) case _error_code_: _output_message_ = #_error_code_; break;

namespace xsix
{
	namespace socketapi
	{
		void network_env_init()
		{
#if defined(_XSIX_WINDOWS)
			WSADATA wsaData;
			XASSERT(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#endif
		}

		void network_env_cleanup()
		{
#if defined(_XSIX_WINDOWS)
			WSACleanup();
#endif
		}

		std::string get_socket_error_msg()
		{
			std::string msg = "UNKNOWN";
			int32_t rc = -1;

#if   defined (_XSIX_WINDOWS)
			rc = WSAGetLastError();
			switch (rc)
			{
				__XSIX_SOCK_ERROR_MSG(WSANOTINITIALISED, msg);
				__XSIX_SOCK_ERROR_MSG(WSAENETDOWN, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEAFNOSUPPORT, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEINPROGRESS, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEMFILE, msg);
				__XSIX_SOCK_ERROR_MSG(WSAENOBUFS, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEPROTONOSUPPORT, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEPROTOTYPE, msg);
				__XSIX_SOCK_ERROR_MSG(WSAESOCKTNOSUPPORT, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEADDRINUSE, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEADDRNOTAVAIL, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEFAULT, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEINVAL, msg);
				__XSIX_SOCK_ERROR_MSG(WSAENOTSOCK, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEINTR, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEALREADY, msg);
				__XSIX_SOCK_ERROR_MSG(WSAECONNREFUSED, msg);
				__XSIX_SOCK_ERROR_MSG(WSAENETUNREACH, msg);
				__XSIX_SOCK_ERROR_MSG(WSAETIMEDOUT, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEWOULDBLOCK, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEOPNOTSUPP, msg);
				__XSIX_SOCK_ERROR_MSG(WSAENOPROTOOPT, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEACCES, msg);
				__XSIX_SOCK_ERROR_MSG(WSAENETRESET, msg);
				__XSIX_SOCK_ERROR_MSG(WSAENOTCONN, msg);
				__XSIX_SOCK_ERROR_MSG(WSAESHUTDOWN, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEMSGSIZE, msg);
				__XSIX_SOCK_ERROR_MSG(WSAEHOSTUNREACH, msg);
				__XSIX_SOCK_ERROR_MSG(WSAECONNABORTED, msg);
				__XSIX_SOCK_ERROR_MSG(WSAECONNRESET, msg);
			default:
				break;
			}

#elif defined(_XSIX_LINUX)
			rc = errno;
			switch (rc)
			{
				__XSIX_SOCK_ERROR_MSG(EALREADY, msg);
				__XSIX_SOCK_ERROR_MSG(ECONNABORTED, msg);
				__XSIX_SOCK_ERROR_MSG(ECONNREFUSED, msg);
				__XSIX_SOCK_ERROR_MSG(ECONNRESET, msg);
				__XSIX_SOCK_ERROR_MSG(ETIMEDOUT, msg);
				__XSIX_SOCK_ERROR_MSG(EFAULT, msg);
				__XSIX_SOCK_ERROR_MSG(ENOTSOCK, msg);
				__XSIX_SOCK_ERROR_MSG(EINPROGRESS, msg);
				__XSIX_SOCK_ERROR_MSG(EWOULDBLOCK, msg);
				__XSIX_SOCK_ERROR_MSG(EREMOTE, msg);
			default:
				break;
			}
#endif
			return msg;

		}

		bool set_socket_opt(SOCKET fd, int32_t socktype, int32_t opname, void* opval, uint32_t oplen)
		{
#if defined(_XSIX_WINDOWS)

			int32_t rc = setsockopt(fd, socktype, opname, (char*)opval, oplen);
			if (rc == SOCKET_ERROR)
			{
				log_socket_error();
				return false;
			}
			return true;

#elif defined(_XSIX_LINUX)

			int32_t rc = setsockopt(fd, socktype, opname, opval, oplen);
			if (rc == SOCKET_ERROR)
			{
				log_socket_error();
				return false;
			}
			return true;
#endif
			return false;
		}

		void log_socket_error()
		{
			printf("socket error : %s", get_socket_error_msg().c_str());
		}

		void close_socket(SOCKET fd)
		{
			if (fd == INVALID_SOCKET)
			{
				return;
			}
#if defined(_XSIX_WINDOWS)
			closesocket(fd);
#else 
			close(fd);
#endif
		}

		SOCKET create_socket(bool use_ipv6 /*= false*/)
		{
			int32_t domain = use_ipv6 ? AF_INET6 : AF_INET;
			SOCKET sock = socket(domain, SOCK_STREAM, 0);
			if (sock == INVALID_SOCKET)
			{
				log_socket_error();
			}
			return sock;
		}

		bool set_nonblock(SOCKET fd, bool on)
		{
			if (fd == INVALID_SOCKET)
			{
				return false;
			}

#if defined(_XSIX_WINDOWS)

			unsigned long ul = on ? 1 : 0;
			int32_t rc = ioctlsocket(fd, FIONBIO, (unsigned long*)&ul);
			if (rc < 0)
			{
				log_socket_error();
				return false;
			}
			return true;

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
				log_socket_error();
				return false;
			}
			return true;
#endif

			return false;
		}

		bool set_reuse_addr(SOCKET fd)
		{
			int32_t opval = 1;
			return set_socket_opt(fd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(opval));
		}

		bool set_reuse_port(SOCKET fd)
		{
#ifdef SO_REUSEPORT
			int32_t opval = 1;
			return set_socket_opt(fd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(opval));
#endif
			return false;
		}

		bool set_linger_ex(SOCKET fd, uint32_t linger_time)
		{
			struct linger socklinger;
			socklinger.l_onoff = linger_time > 0 ? 1 : 0;
			socklinger.l_linger = linger_time > 0 ? linger_time : 0;
			return set_socket_opt(fd, SOL_SOCKET, SO_LINGER, &socklinger, sizeof(socklinger));
		}

		bool set_tcp_nodelay(SOCKET fd, bool on)
		{
			int32_t opval = on ? 1 : 0;
			return set_socket_opt(fd, IPPROTO_TCP, TCP_NODELAY, &opval, sizeof(opval));
		}

		bool bind_ex(SOCKET fd, uint16_t port, bool use_ipv6 /*= false*/)
		{
			if (use_ipv6)
			{
				sockaddr_in6 addr;
				memset(&addr, 0, sizeof(addr));

				addr.sin6_family = AF_INET6;
				addr.sin6_port = htons(port);
				addr.sin6_addr = in6addr_any;
				int32_t rc = bind(fd, (const sockaddr*)&addr, sizeof(addr));
				if (rc == SOCKET_ERROR)
				{
					log_socket_error();
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
					log_socket_error();
					return false;
				}
				return true;
			}
		}

		bool listen_ex(SOCKET fd, int32_t nBacklog)
		{
			int32_t rc = listen(fd, nBacklog);
			if (rc == SOCKET_ERROR)
			{
				log_socket_error();
				return false;
			}
			return true;
		}

		int32_t recvbytes(SOCKET fd, char* buffer, int32_t cnt)
		{
			if (fd == INVALID_SOCKET)
			{
				return SOCKET_ERROR;
			}

			if (!buffer || cnt <= 0)
			{
				log_socket_error();
				return SOCKET_ERROR;
			}

			auto rc = recv(fd, buffer, cnt, 0);
			if (rc == SOCKET_ERROR)
			{
				log_socket_error();
				return SOCKET_ERROR;
			}

			return rc;
		}

		int32_t sendbytes(SOCKET fd, const char* buffer, int32_t cnt)
		{
			if (fd == INVALID_SOCKET)
			{
				return SOCKET_ERROR;
			}

			auto rc = send(fd, buffer, cnt, 0);
			if (rc == SOCKET_ERROR)
			{
				log_socket_error();
				return SOCKET_ERROR;
			}

			return rc;
		}

		bool is_ipv4_addr(const std::string& ip)
		{
			struct sockaddr_in addr;
			memset(&addr, 0, sizeof(addr));
			int32_t rc = inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
			return rc == 0 ? true : false;
		}

		bool is_ipv6_addr(const std::string& ip)
		{
			struct sockaddr_in6 addr;
			memset(&addr, 0, sizeof(addr));
			int32_t rc = inet_pton(AF_INET6, ip.c_str(), &addr.sin6_addr);
			return rc == 0 ? true : false;
		}

		void socketapi::shutdown_write(SOCKET fd)
		{
#if defined(_XSIX_WINDOWS)
			shutdown(fd, SD_SEND);
#elif defined(_XSIX_LINUX)
			shutdown(fd, SHUT_WR);
#endif
		}

		void socketapi::shutdown_read(SOCKET fd)
		{
#if defined(_XSIX_WINDOWS)
			shutdown(fd, SD_RECEIVE);
#elif defined(_XSIX_LINUX)
			shutdown(fd, SHUT_RD);
#endif
		}

		void socketapi::shutdown_both(SOCKET fd)
		{
#if defined(_XSIX_WINDOWS)
			shutdown(fd, SD_BOTH);
#elif defined(_XSIX_LINUX)
			shutdown(fd, SHUT_RDWR);
#endif
		}
	}
}