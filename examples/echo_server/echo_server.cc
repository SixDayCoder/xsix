#include "xsix/network/network.h"
#include "xsix/time/timestamp.h"
#include "xsix/block_queue.hpp"
#include "xsix/network/eventloop.h"
#include "xsix/network/tcp_server.h"
#include <stdio.h>

static void on_echo_message(const xsix::TCPConnPtr& conn, xsix::buffer* buf, xsix::Timestamp ts)
{
	conn->send(buf);
}

int main()
{
	xsix::socketapi::network_env_init();

	xsix::EventLoop eventloop;

	xsix::TCPServer tcp_server(&eventloop, 8888);

	tcp_server.set_conn_recv_message_cb(on_echo_message);

	tcp_server.run();

	eventloop.run();

	xsix::socketapi::network_env_cleanup();

	return 0;
}