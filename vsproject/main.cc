#include "xsix/network/network.h"
#include "xsix/time/timestamp.h"
#include "xsix/block_queue.hpp"
#include "xsix/network/eventloop.h"
#include <stdio.h>

int main()
{
	xsix::socketapi::network_env_init();
	printf("hello xsix\n");
	xsix::EventLoop eventloop;
	eventloop.run();
	getchar();
	xsix::socketapi::network_env_cleanup();
	return 0;
}