#include <stdio.h>

#include "asio.hpp"

#include "xsix/time/timestamp.h"
#include "xsix/block_queue.hpp"

#include "xsix/network/tcp_server.h"

int main()
{
	asio::io_context ctx;
	xsix::TCPServer server(ctx, 8888);
	server.set_reuse_addr(true);
	server.accept();
	server.loop();

	return 0;
}