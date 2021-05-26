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
	ptr->send(msg.c_str(), msg.length());
}

void echo(xsix::TCPClientPtr conn)
{
	if (!conn)
	{
		return;
	}

	if (conn->m_recv_buffer.empty())
	{
		return;
	}

	char buf[4096] = { 0 };
	int32_t size = conn->m_recv_buffer.write_to(buf, 4096);
	if (size > 0)
	{
		printf("tcp client recv : %s\n", buf);
		conn->send(buf, size);
	}

	std::this_thread::sleep_for(std::chrono::microseconds(50));
}

int main()
{
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 8888);
	std::vector<std::thread> thread_vec;

	for (int32_t i = 0; i < 20; ++i)
	{
		xsix::TCPClientPtr client(new xsix::TCPClient());
		client->set_connected_handler(connected);
		client->set_message_handler(echo);
		client->async_connect(ep);

		std::thread th(std::bind(&xsix::TCPClient::loop, client));
		thread_vec.emplace_back(std::move(th));		
	}

	for (auto& th : thread_vec)
	{
		th.join();
	}

	return 0;
}