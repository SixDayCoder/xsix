#pragma once

#include "xsix/common_define.h"
#include "xsix/noncopyable.h"
#include <string>

//network define
#if defined(_XSIX_WINDOWS)

#include <WinSock2.h>
#include <WS2tcpip.h>

// /*
//  * Windows Sockets errors redefined as regular Berkeley error constants.
//  * Copied from winsock.h
//  */

// #define EWOULDBLOCK             WSAEWOULDBLOCK
// #define EINPROGRESS             WSAEINPROGRESS
// #define EALREADY                WSAEALREADY
// #define ENOTSOCK                WSAENOTSOCK
// #define EDESTADDRREQ            WSAEDESTADDRREQ
// #define EMSGSIZE                WSAEMSGSIZE
// #define EPROTOTYPE              WSAEPROTOTYPE
// #define ENOPROTOOPT             WSAENOPROTOOPT
// #define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
// #define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
// #define EOPNOTSUPP              WSAEOPNOTSUPP
// #define EPFNOSUPPORT            WSAEPFNOSUPPORT
// #define EAFNOSUPPORT            WSAEAFNOSUPPORT
// #define EADDRINUSE              WSAEADDRINUSE
// #define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
// #define ENETDOWN                WSAENETDOWN
// #define ENETUNREACH             WSAENETUNREACH
// #define ENETRESET               WSAENETRESET
// #define ECONNABORTED            WSAECONNABORTED
// #define ECONNRESET              WSAECONNRESET
// #define ENOBUFS                 WSAENOBUFS
// #define EISCONN                 WSAEISCONN
// #define ENOTCONN                WSAENOTCONN
// #define ESHUTDOWN               WSAESHUTDOWN
// #define ETOOMANYREFS            WSAETOOMANYREFS
// #define ETIMEDOUT               WSAETIMEDOUT
// #define ECONNREFUSED            WSAECONNREFUSED
// #define ELOOP                   WSAELOOP
// #define ENAMETOOLONG            WSAENAMETOOLONG
// #define EHOSTDOWN               WSAEHOSTDOWN
// #define EHOSTUNREACH            WSAEHOSTUNREACH
// #define ENOTEMPTY               WSAENOTEMPTY
// #define EPROCLIM                WSAEPROCLIM
// #define EUSERS                  WSAEUSERS
// #define EDQUOT                  WSAEDQUOT
// #define ESTALE                  WSAESTALE
// #define EREMOTE                 WSAEREMOTE
// #define EAGAIN 				   EWOULDBLOCK 

#endif

#if defined(_XSIX_LINUX)

#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>

typedef int	int;

#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)  
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)  
#endif

#if defined(_XSIX_WINDOWS)
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wsock32.lib")
#endif


#define XSIX_NETWORK_INIT   xsix::socketapi::network_env_init();
#define XSIX_NETWORK_CLEAN  xsix::socketapi::network_env_cleanup();
#define XSIX_DEFAULT_LISTEN_BACKLOG (128)

namespace xsix
{

	namespace socketapi
	{
		std::string	get_socket_error_msg();

		void		network_env_init();

		void		network_env_cleanup();

		bool		set_socket_opt(int fd, int32_t socktype, int32_t opname, void* opval, uint32_t oplen);

		void		log_socket_error();

		void		close_socket(int fd);

		int			create_socket(bool use_ipv6 = false);

		bool		set_nonblock(int fd, bool on);

		bool		set_keep_alive(int fd);

		bool		set_reuse_addr(int fd);

		bool		set_reuse_port(int fd);

		bool		set_linger_ex(int fd, uint32_t linger_time);

		bool		set_tcp_nodelay(int fd, bool on);

		bool		bind_ex(int fd, uint16_t port, bool use_ipv6 = false);

		bool		listen_ex(int fd, int32_t backlog);

		int32_t		recvbytes(int fd, char* buffer, int32_t cnt);

		int32_t		sendbytes(int fd, const char* buffer, int32_t cnt);

		void		shutdown_write(int fd);

		void		shutdown_read(int fd);

		void		shutdown_both(int fd);

		bool		is_ipv4_addr(const std::string& ip);

		bool		is_ipv6_addr(const std::string& ip);	
	}

	class NetAddr
	{
	public:

		explicit NetAddr(const std::string& ip, uint16_t port, bool ipv6 = false);

		NetAddr(bool ipv6 = false) : m_ipv6(ipv6) { memset(&m_ss, 0, sizeof(m_ss)); }

	public:

		std::string				ipaddress() const;

		uint16_t				port() const;

		bool					get_ipv6() const { return m_ipv6; }

		void					set_ipv6(bool ipv6) { m_ipv6 = ipv6; }

		void					set_sockaddr_storage(struct sockaddr_storage ss) { m_ss = ss; }

		struct sockaddr_storage get_sockaddr_storage() const { return m_ss; }

	private:

		struct sockaddr_storage		m_ss;
		bool						m_ipv6;	
	};

	class TCPSocket : public INonCopyable
	{
	public:

		explicit TCPSocket(bool use_ipv6 = false) : m_ipv6(use_ipv6) { m_fd = socketapi::create_socket(use_ipv6); }

		explicit TCPSocket(int fd, bool use_ipv6 = false) : m_fd(fd), m_ipv6(use_ipv6) {}

		virtual ~TCPSocket() {}

	public:

		int		get_sockfd() const { return m_fd; };

		bool	set_nonblock(bool on) { return socketapi::set_nonblock(m_fd, on); }

		bool	set_nodelay(bool on) { return socketapi::set_tcp_nodelay(m_fd, on); }

		bool	set_keep_alive() { return socketapi::set_keep_alive(m_fd); }

		bool	is_valid() const { return m_fd != INVALID_SOCKET; }

		void	close() { socketapi::close_socket(m_fd); }

		void    shutdown_read() { socketapi::shutdown_read(m_fd); }
		
		void    shutdown_write() { socketapi::shutdown_read(m_fd); }

		void    shutdown_both() { socketapi::shutdown_read(m_fd); }

	public:

		int32_t recvbytes(char* buffer, int32_t cnt) { return socketapi::recvbytes(m_fd, buffer, cnt); }

		int32_t sendbytes(const char* buffer, int32_t cnt) { return socketapi::sendbytes(m_fd, buffer, cnt); }

	protected:

		bool   m_ipv6;

		int		m_fd;
	};

	class TCPClientSocket : public TCPSocket
	{
	public:

		explicit TCPClientSocket(bool use_ipv6 = false) : TCPSocket(use_ipv6) {}

	public:

		bool connect_ex(const NetAddr& addr);
	};

	class TCPServerSocket : public TCPSocket
	{
	public:

		explicit TCPServerSocket(bool use_ipv6 = false) : TCPSocket(use_ipv6), m_port(0) {}

	public:

		bool	set_reuse_port() { return socketapi::set_reuse_port(m_fd); };

		bool	set_reuse_addr() { return socketapi::set_reuse_addr(m_fd); };

		bool	bind_ex(uint16_t port) { m_port = port; return socketapi::bind_ex(m_fd, m_port, m_ipv6); }

		bool	listen_ex(int32_t backlog = XSIX_DEFAULT_LISTEN_BACKLOG) { return socketapi::listen_ex(m_fd, backlog); }

		int		accept_ex(NetAddr* addr);

	private:

		uint16_t m_port;
	};
}