#include "xsix/xsix_tcp_server.h"
#include <stdio.h>

#define TEST_BIND_PORT (8888)

int main()
{
	XSIX_NETWORK_INIT;

	xsix::TCPServer server(TEST_BIND_PORT);
	XASSERT(server.Listen());

	server.Run();

	getchar();

	XSIX_NETWORK_CLEAN;
	return 0;
}