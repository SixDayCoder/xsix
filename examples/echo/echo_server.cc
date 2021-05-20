#include <stdio.h>
#include "xsix/network/tcp_server.h"

void conn_close(xsix::TCPConnPtr conn, const asio::error_code& ec)
{
	if (!conn)
	{
		return;
	}
	printf("conn closed! id : %d, error : %s\n", conn->get_id(), ec.message().c_str());
}

void conn_echo(xsix::TCPConnPtr conn)
{
	if (!conn)
	{
		return;
	}

	char buf[4096] = { 0 };
	int32_t size = conn->m_recv_buffer.write_to(buf, 4096);
	if (size > 0)
	{
		printf("tcp conn id : %d, recv : %s\n", conn->get_id(), buf);
		conn->send(buf, size);
	}
}

void accept_handler(xsix::TCPServerPtr server, xsix::TCPConnPtr conn, const asio::error_code& ec)
{
	if (!server || !conn)
	{
		return;
	}

	printf("new conn (id:%d, ip:%s, port:%d)\n",
		conn->get_id(),
		conn->remote_ip().c_str(),
		conn->remote_port()
	);
}

int main()
{
	asio::io_context ctx;
	xsix::TCPServerPtr server(new xsix::TCPServer(ctx, 8888));

	server->set_reuse_addr(true);
	server->set_accept_handler(accept_handler);
	server->set_conn_message_handler(conn_echo);
	server->set_conn_close_handler(conn_close);

	server->accept();
	server->loop();

	return 0;
}