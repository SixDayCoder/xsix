#include "chat_room_msg.hpp"
#include "xsix/actor/actor_core.h"
#include "xsix/network/tcp_client.h"

#include <iostream>
#include <functional>
#include <chrono>
using namespace std;


std::thread loop_thread;
std::thread input_thread;

void fetch_input(xsix::TCPClientPtr client)
{
	std::string s;
	while (true)
	{
		s.clear();
		cout << "input msg : >";
		cin >> s;

		chat_room_msg msg;
		msg.set_content(s.c_str(), s.length());

		int32_t bytes = 0;
		char sendbuf[1024] = { 0 };
		msg.write_to_buf(sendbuf, &bytes);

		client->send(sendbuf, bytes);
	}
}

void connected(xsix::TCPClientPtr ptr)
{
	if (!ptr)
	{
		return;
	}
	cout << "connected ready input!" << endl;
	input_thread = std::thread(std::bind(fetch_input, ptr));
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

	//chat_room_msg msg;
	//conn->m_recv_buffer.peek(&msg.header, sizeof(chat_room_msg_header));

	//uint16_t content_length = 0;
	//memcpy(&content_length, &msg.header, sizeof(uint16_t));
	//content_length = ntohs(content_length);

	//if (conn->m_recv_buffer.length() >= content_length &&
	//	conn->m_recv_buffer.skip(sizeof(chat_room_msg_header)))
	//{
	//	conn->m_recv_buffer.write_to(msg.content, content_length);
	//	printf("%s\n", msg.content);
	//}

	//parse message
	std::string s = conn->m_recv_buffer.retrieve_all_as_string();
	conn->m_recv_buffer.clear();

	chat_room_msg msg;
	memcpy(&msg.header, s.c_str(), sizeof(chat_room_msg_header));

	uint16_t content_length = 0;
	memcpy(&content_length, &msg.header, sizeof(uint16_t));
	content_length = ntohs(content_length);
	msg.header.size = content_length;

	if (s.length() >= content_length + sizeof(chat_room_msg_header))
	{
		memcpy(msg.content, s.c_str() + sizeof(chat_room_msg_header), content_length);
		printf("%s\n", msg.content);
	}
}

int main()
{
	asio::io_context ctx;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 9999);
	xsix::TCPClientPtr client(new xsix::TCPClient(ctx));

	client->set_connected_handler(connected);
	client->set_message_handler(chat);
	client->connect(ep);

	loop_thread = std::thread(std::bind(&xsix::TCPClient::loop, client));
	loop_thread.join();
	input_thread.join();
	
	return 0;
}
