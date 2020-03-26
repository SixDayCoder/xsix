#pragma once

#include "xsix/xsix_common_define.h"
#include <string>

//network define
#if defined(_XSIX_WINDOWS)

	#include <WinSock2.h>
	#include <WS2tcpip.h>

#elif defined(_XSIX_LINUX)

	#include <unistd.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <fcntl.h>
	#include <poll.h>

	typedef int	SOCKET;

#ifndef INVALID_SOCKET
	#define INVALID_SOCKET (-1)  
#endif

#ifndef SOCKET_ERROR
	#define SOCKET_ERROR (-1)  
#endif

#endif

#ifndef _XSIX_SOCK_ERROR_MSG

	#define _XSIX_SOCK_ERROR_MSG(name, msg) \
		case name:\
			msg = #name;\
			break;

#endif

#if defined(_XSIX_WINDOWS)
	#pragma comment(lib, "Ws2_32.lib")
#endif


namespace xsix
{
	class NetworkEnv
	{
	public:

		static void Init();

		static void Clean();
	};

	class SocketAPI
	{
	public:

		static void LogSocketError();

		static void CloseSocket(SOCKET fd);

		static SOCKET CreateSocket(bool bUseIPv6 = false);

		static bool SetNonBlock(SOCKET fd, bool on);

		static bool SetReuseAddr(SOCKET fd);

		static bool SetReusePort(SOCKET fd);

		static bool SetLinger(SOCKET fd, uint32_t nLingerTime);

		static bool SetTCPNoDelay(SOCKET fd, bool on);

		static bool Bind(SOCKET fd, uint16_t port, bool bUseIPv6 = false);

		static bool Listen(SOCKET fd, int32_t nBacklog);

		static int32_t RecvBytes(SOCKET fd, char* buffer, int32_t nCount);

		static int32_t SendBytes(SOCKET fd, const char* buffer, int32_t nCount);

	};

	class NetAddr;

	class Socket;

	class ClientSocket;

	class ServerSocket;

	class NetAddr
	{
	public:

		friend class Socket;

		friend class ClientSocket;

		friend class ServerSocket;

	public:

		explicit NetAddr(const std::string& ip, uint16_t port, bool bUseIPv6 = false);

		NetAddr(bool bUseIPv6 = false);

	public:

		std::string IP();

		uint16_t    Port();

	public:

		static bool IsIPv4Addr(const std::string& ip);

		static bool IsIPv6Addr(const std::string& ip);

	private:

		bool ipv6;

		//struct used only for storage
		//access it by using hints or cast to some specific address type
		struct sockaddr_storage ss;
	};

	class Socket
	{
	public:

		explicit Socket(SOCKET fd, bool bUseIPv6 = false) :
				 m_fd(fd),
				 m_bUseIPv6(bUseIPv6) {}

		explicit Socket(bool bUseIPv6 = false);

		Socket(const Socket & other) = delete;

		Socket & operator = (const Socket & other) = delete;
		
	public:

		bool SetNonBlock(bool on);

		bool SetTCPNoDelay(bool on);

		SOCKET GetSockfd() const { return m_fd; };

		void Close();

	public:
		
		int32_t RecvBytes(char* buffer, int32_t nCount);

		int32_t SendBytes(const char* buffer, int32_t nCount);

	protected:

		bool   m_bUseIPv6;

		SOCKET m_fd;	
	};

	class ClientSocket : public Socket
	{
	public:

		explicit ClientSocket(bool bUseIPv6 = false) : Socket(bUseIPv6) {}
		
		virtual ~ClientSocket() {} 

		ClientSocket(const ClientSocket & other) = delete;

		ClientSocket & operator = (const ClientSocket & other) = delete;
		
	public:

		bool Connect(const NetAddr& addr);

	};

	class ServerSocket : public Socket
	{
	public:

		explicit ServerSocket(bool bUseIPv6 = false) : Socket(bUseIPv6) {}

		virtual ~ServerSocket() {}

		ServerSocket(const ServerSocket & other) = delete;

		ServerSocket & operator = (const ServerSocket & other) = delete;

	public:

		bool SetReusePort();

		bool SetReuseAddr();

		bool Bind(uint16_t port);

		bool Listen(int32_t backlog = 128);

		SOCKET Accept(NetAddr* addr);

	private:

		uint16_t m_Port;

	};
}