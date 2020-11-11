#include "xsix/network/network.h"
#include "xsix/time/timestamp.h"
#include "xsix/block_queue.hpp"
#include "xsix/network/eventloop.h"
#include "xsix/network/tcp_server.h"
#include "xsix/fmt.hpp"
#include "xsix/log/log_define.h"
#include <stdio.h>

static void on_echo_message(const xsix::TCPConnPtr& conn, xsix::buffer* buf, xsix::Timestamp ts)
{
	conn->send(buf);
}

static void simpl_echo_tcp_server(int port)
{
	xsix::socketapi::network_env_init();

	xsix::EventLoop eventloop;

	xsix::TCPServer tcp_server(&eventloop, port);

	tcp_server.set_conn_recv_message_cb(on_echo_message);

	tcp_server.run();

	eventloop.run();

	xsix::socketapi::network_env_cleanup();
}

int main()
{
	simpl_echo_tcp_server(8888);
	return 0;
}