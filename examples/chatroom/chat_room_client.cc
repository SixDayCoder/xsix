#include "chat_room_msg.hpp"
#include "xsix/actor/actor_core.h"
#include "xsix/network/tcp_client.h"

#include <iostream>
#include <functional>
#include <chrono>
#include <random>
#include <atomic>
using namespace std;

asio::io_context ctx;

std::string random_str()
{
	std::string str = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	std::random_device rdv;	
	static std::default_random_engine e{ rdv() };
	static std::uniform_int_distribution<uint32_t> u1(10, 20);
	static std::uniform_int_distribution<uint32_t> u2(0, str.length());

	auto cnt = u1(e);
	std::string result("empty");
	for (int32_t i = 0; i < cnt; ++i)
	{
		int32_t index = u2(e);
		result.push_back(str[index]);
	}
	return result;
}

void generate_msg(xsix::TCPClientPtr client)
{
	std::default_random_engine e;
	std::uniform_int_distribution<uint32_t> u(1, 3);

	std::string s = random_str();
	chat_room_msg msg;
	msg.set_content(s.c_str(), s.length());

	int32_t bytes = 0;
	char sendbuf[1024] = { 0 };
	msg.write_to_buf(sendbuf, &bytes);

	client->send(sendbuf, bytes);
	client->get_context().post(std::bind(generate_msg, client));
	//TODO:massive message hanlder
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void connected(xsix::TCPClientPtr ptr)
{
	if (!ptr)
	{
		return;
	}
	cout << "connected ready input!" << endl;
	ptr->get_context().post(std::bind(generate_msg, ptr));
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
			break;
		}

		msg.header.contentsize = ntohs(msg.header.contentsize);
		if (msg.header.contentsize <= 0 || conn->m_recv_buffer.length() < msg.header.contentsize)
		{
			break;
		}

		conn->m_recv_buffer.skip(sizeof(chat_room_msg_header));
		writesize = conn->m_recv_buffer.write_to(msg.content, msg.header.contentsize);
		std::cout << msg.content << std::endl;
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
