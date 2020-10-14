#include "xsix/network/network.h"
#include "xsix/time/timestamp.h"
#include "xsix/block_queue.hpp"
#include "xsix/network/eventloop.h"
#include "xsix/network/tcp_server.h"
#include <stdio.h>

int main()
{
	xsix::socketapi::network_env_init();

	xsix::EventLoop eventloop;

	xsix::TCPServer tcp_server(&eventloop, 8888);
	
	tcp_server.run();

	eventloop.run();

	xsix::socketapi::network_env_cleanup();
	return 0;
}