#include <stdio.h>
#include <time.h>
#include <thread>
#include <iostream>

#include "xsix/network/tcp_client.h"


void connected(xsix::TCPClientPtr ptr)
{
	if (!ptr)
	{
		return;
	}
	std::string msg = "sixday";
	printf("connected! ready send msg : %s, size : %d\n", msg.c_str(), msg.size());
	ptr->send(msg.c_str(), msg.size());
}

void echo(xsix::TCPClientPtr conn)
{
	if (!conn)
	{
		return;
	}

	std::string msg = conn->m_recv_buffer.retrieve_all_as_string();
	conn->m_recv_buffer.clear();
	if (msg.size() > 0)
	{
		printf("tcp client recv : %s\n", msg.c_str());
		conn->send(msg.c_str(), msg.size());
	}
}

int main()
{
	std::vector<std::thread> thread_vec;

	for (int32_t i = 0; i < 20; ++i)
	{
		asio::io_context ctx;
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 8888);
		xsix::TCPClientPtr client(new xsix::TCPClient(ctx));

		client->set_connected_handler(connected);
		client->set_message_handler(echo);
		client->connect(ep);

		std::thread th(std::bind(&xsix::TCPClient::loop, client));
		thread_vec.emplace_back(std::move(th));
	}

	for (auto& th : thread_vec)
	{
		th.join();
	}

	return 0;
}