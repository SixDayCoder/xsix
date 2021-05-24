#include "chat_room_msg.hpp"
#include "xsix/actor/actor_core.h"
#include "xsix/network/tcp_client.h"

#include <iostream>
#include <functional>
#include <chrono>
using namespace std;

void send_random_msg(xsix::TCPClientPtr client)
{
	chat_room_msg msg = chat_room_msg::get_random_msg();
	int32_t bytes = 0;
	char sendbuf[1024] = { 0 };
	msg.write_to_buf(sendbuf, &bytes);
	client->send(sendbuf, bytes);
}

void connected(xsix::TCPClientPtr ptr)
{
	if (!ptr)
	{
		return;
	}
	cout << "connected ready input!" << endl;
	send_random_msg(ptr);
}

void chat(xsix::TCPClientPtr conn)
{
	if (!conn)
	{
		return;
	}
	
	if (conn->m_recv_buffer.length() < sizeof(chat_room_msg_header))
	{
		return;
	}

	int32_t peeksize = 0;
	int32_t writesize = 0;
	chat_room_msg msg;
	while (!conn->m_recv_buffer.empty())
	{
		msg.clear();

		peeksize = conn->m_recv_buffer.peek(&msg.header, sizeof(chat_room_msg_header));
		if (peeksize != sizeof(chat_room_msg_header))
		{
			std::cout <<  "peeksize error" << std::endl;
			break;
		}

		msg.header.contentsize = ntohs(msg.header.contentsize);
		if (msg.header.contentsize <= 0 || 
			conn->m_recv_buffer.length() < msg.header.contentsize + sizeof(chat_room_msg_header))
		{
			std::cout <<  "client content wrong : " << msg.header.contentsize << " : " << conn->m_recv_buffer.length() << std::endl;
			break;
		}

		XASSERT(conn->m_recv_buffer.skip(sizeof(chat_room_msg_header)));
		XASSERT(conn->m_recv_buffer.write_to(msg.content, msg.header.contentsize));

		std::cout << msg.content << std::endl;
		send_random_msg(conn);
	}
}

int main()
{
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 9999);
	xsix::TCPClientPtr client(new xsix::TCPClient());

	client->set_connected_handler(connected);
	client->set_message_handler(chat);
	client->async_connect(ep);
	client->loop();
	
	return 0;
}
