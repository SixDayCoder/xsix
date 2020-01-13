#pragma once

#include <stdint.h>
#include <assert.h>


//assert define
namespace xsix
{
	#define XASSERT(expression) assert(expression)
}

//memory define
namespace xsix
{
	#define XMALLOC  malloc
	#define XFREE    free
	#define XREALLOC realloc
}

//network define
#if defined(_XSIX_WINDOWS)

#include <WinSock2.h>
#include <WS2tcpip.h>
namespace xsix
{
	using Sockfd = SOCKET;
}

#elif defined(_XSIX_LINUX)

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#define     INVALID_SOCKET   (-1)
#define		SOCKET_ERROR	 (-1)
namespace xsix
{
	using Sockfd = int32_t;
}

#endif